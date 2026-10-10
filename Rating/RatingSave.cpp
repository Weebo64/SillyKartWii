#include <kamek.hpp>
#include <IO/IO.hpp>
#include <PulsarSystem.hpp>
#include <Network/Rating/PlayerRating.hpp>
#include <MarioKartWii/RKSYS/RKSYSMgr.hpp>
#include <MarioKartWii/System/Rating.hpp>

/*
    PlayerRating.hpp
    Copyright (C) 2025 ZPL

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

namespace Pulsar {
namespace PointRating {

struct CacheEntry {
    u16 vr;
    u16 br;
    u16 originalVr;
    u16 originalBr;
    bool hasData;
    bool hasOriginal;
};

struct PackedHeader {
    u32 magic;
    u16 version;
    u16 count;
};

struct PackedEntry {
    u16 vr;
    u16 br;
    u16 flags;
};

static const u32 kMagic = 'VRBR';
static const u16 kVersion = 1;
static const u32 kMaxLicenses = 4;
static CacheEntry sCache[kMaxLicenses] = {};
static bool sLoaded = false;
static char sFilePath[IOS::ipcMaxPath] = {0};

static const char* GetFilePath() {
    if (sFilePath[0] == '\0') {
        const System* system = System::sInstance;
        if (system == nullptr) return nullptr;
        snprintf(sFilePath, IOS::ipcMaxPath, "%s/%s", system->GetModFolder(), "IKWRating.pul");
        sFilePath[IOS::ipcMaxPath - 1] = '\0';
    }
    return sFilePath;
}

static void EnsureLoaded() {
    if (sLoaded) return;
    IO* io = IO::sInstance;
    const char* path = GetFilePath();
    if (io == nullptr || path == nullptr) return;

    if (io->OpenFile(path, FILE_MODE_READ)) {
        PackedHeader header = {};
        const s32 readHeader = io->Read(sizeof(header), &header);
        if (readHeader == sizeof(header) && header.magic == kMagic && header.version == kVersion) {
            const u16 count = header.count <= kMaxLicenses ? header.count : kMaxLicenses;
            for (u16 idx = 0; idx < count; ++idx) {
                PackedEntry entry = {};
                if (io->Read(sizeof(entry), &entry) != static_cast<s32>(sizeof(entry))) {
                    break;
                }
                if ((entry.flags & 0x1) != 0) {
                    sCache[idx].vr = entry.vr;
                    sCache[idx].br = entry.br;
                    sCache[idx].hasData = true;
                }
            }
        }
        io->Close();
    }
    sLoaded = true;
}

static void Persist() {
    IO* io = IO::sInstance;
    const char* path = GetFilePath();
    if (io == nullptr || path == nullptr) return;

    struct PackedFile {
        PackedHeader header;
        PackedEntry entries[kMaxLicenses];
    } file = {};

    file.header.magic = kMagic;
    file.header.version = kVersion;
    file.header.count = kMaxLicenses;

    for (u32 idx = 0; idx < kMaxLicenses; ++idx) {
        const CacheEntry& entry = sCache[idx];
        file.entries[idx].vr = entry.vr;
        file.entries[idx].br = entry.br;
        file.entries[idx].flags = entry.hasData ? 0x1 : 0x0;
    }

    if (!io->OpenFile(path, FILE_MODE_WRITE)) {
        io->CreateAndOpen(path, FILE_MODE_WRITE);
    }
    io->Overwrite(sizeof(file), &file);
    io->Close();
}

static u16 ClampRating(u16 value) {
    if (value < MinRating) return MinRating;
    if (value > MaxRating) return MaxRating;
    return value;
}

static void ApplyToLicense(u32 licenseIdx, RKSYS::LicenseMgr& license) {
    EnsureLoaded();
    if (licenseIdx >= kMaxLicenses) return;

    CacheEntry& entry = sCache[licenseIdx];

    if (!entry.hasData) {
        entry.vr = 100;
        entry.br = 100;
        entry.hasData = true;
        Persist();
        license.vr.points = 100;
        license.br.points = 100;
        return;
    }

    entry.originalVr = ClampRating(license.vr.points);
    entry.originalBr = ClampRating(license.br.points);
    entry.hasOriginal = true;

    license.vr.points = ClampRating(entry.vr);
    license.br.points = ClampRating(entry.br);
}

static void StoreFromLicense(u32 licenseIdx, const RKSYS::LicenseMgr& license) {
    EnsureLoaded();
    if (licenseIdx >= kMaxLicenses) return;

    CacheEntry& entry = sCache[licenseIdx];

    if (!entry.hasData) {
        entry.vr = 100;
        entry.br = 100;
    } else {
        const u16 nextVr = ClampRating(license.vr.points);
        const u16 nextBr = ClampRating(license.br.points);
        const bool changed = entry.vr != nextVr || entry.br != nextBr;
        entry.vr = nextVr;
        entry.br = nextBr;
        if (!changed) return;
    }
    
    entry.hasData = true;
    Persist();
}

extern "C" int SaveManager_ReadLicenseHook() {
    RKSYS::Mgr* mgr = RKSYS::Mgr::sInstance;
    if (mgr != nullptr) {
        RKSYS::LicenseMgr* license = nullptr;
        asm("mr %0, r31" : "=r"(license));
        if (license != nullptr) {
            const u32 base = reinterpret_cast<u32>(&mgr->licenses[0]);
            const u32 addr = reinterpret_cast<u32>(license);
            if (addr >= base) {
                const u32 diff = (addr - base) / sizeof(RKSYS::LicenseMgr);
                if (diff < kMaxLicenses) {
                    ApplyToLicense(diff, *license);
                }
            }
        }
    }
    return 1;
}

extern "C" void SaveManager_WriteLicenseHook(RKSYS::Binary* raw, u32 licenseIdx) {
    RKSYS::Mgr* mgr = RKSYS::Mgr::sInstance;
    if (mgr != nullptr && licenseIdx < kMaxLicenses) {
        StoreFromLicense(licenseIdx, mgr->licenses[licenseIdx]);

        if (raw != nullptr) {
            RKSYS::RKPD& rawLicense = raw->core.licenses[licenseIdx];
            rawLicense.magic = 'RKPD';

            CacheEntry& entry = sCache[licenseIdx];

            rawLicense.vr = entry.hasData ? entry.vr : 100;
            rawLicense.br = entry.hasData ? entry.br : 100;
        }
    }
}
kmCall(0x805455a8, SaveManager_ReadLicenseHook);
kmCall(0x80546f9c, SaveManager_WriteLicenseHook);

}  // namespace PointRating
}  // namespace Pulsar
