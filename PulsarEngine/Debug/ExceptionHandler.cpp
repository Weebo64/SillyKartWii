#include <kamek.hpp>
#include <core/System/SystemManager.hpp>
#include <core/rvl/base/ppc.hpp>
#include <core/rvl/OS/OSthread.hpp>
#include <core/rvl/OS/OSTitle.hpp>
#include <core/rvl/OS/OSBootInfo.hpp>
#include <core/rvl/PAD.hpp>
#include <core/rvl/KPAD.hpp>
#include <core/egg/Exception.hpp>
#include <Debug/Debug.hpp>
#include <PulsarSystem.hpp>
#include <IO/IO.hpp>
#include <SillyKartWii.hpp>
#include <SillyKartWiiChannel.hpp>
#include <SlotExpansion/CupsConfig.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace Debug {

// Declare OSYieldThread from symbols
extern "C" void OSYieldThread();

OS::Thread* crashThread = nullptr;
static u16 crashError = 0;

// SillyKartWii crash data storage
static u8 VanillaMode = 0;
static u8 SillyMode = 0;
static u8 SectionMode = 0;
static u16 LastPlayedTrack = 0;
static u16 SillyEffects = 0;
static u16 SillySounds = 0;
static u8 SillyPhysics = 0;
static u8 BraikeDrifting = 0;
static u16 FreeRoamMode = 0;

using namespace nw4r;

void FatalError(const char* string) {
    GX::Color fg;
    fg.rgba = 0xFFFFFFFF;
    GX::Color bg = {0};
    OS::Fatal(fg, bg, string);
}

#pragma suppress_warnings on
void LaunchSoftware() {
    s32 result = IO::OpenFix("/dev/dolphin", IOS::MODE_NONE);
    if (result >= 0) {
        IOS::Close(result);
        SystemManager::Shutdown();
        return;
    }
    // Try Riivo, then HBC, then Wii Menu
    static const u32 titleIds[2] = {'RIIV', 'LULZ'};
    char tmdPath[IOS::ipcMaxPath];
    for(int i = 0; i < 2; ++i) {
        u32 launchCode = 0x00010001;
        u32 titleId = titleIds[i];
        snprintf(tmdPath, IOS::ipcMaxPath, "/title/%08x/%08x/content/title.tmd", launchCode, titleId);
        result = IO::OpenFix(tmdPath, IOS::MODE_NONE);
        if(result >= 0) {
            ISFS::Close(result);
            OS::__LaunchTitle(launchCode, titleId);
            return;
        }
    }
    OS::__LaunchTitle(0x1, 0x2);  // Launch Wii Menu if channel isn't found
}
#pragma suppress_warnings reset

// Credit Star

//Data Shown
//kmWrite16(0x802A7410, 0x00000023);
//Show Handler
//kmWrite32(0x802A7404, 0x00000000);
//Show StackTrace
kmWrite32(0x80023948, 0x281e0007);
//Max number of lines
kmWrite32(0x80009324, 0x38800068);

//Lines on the screen and x-pos
static void SetConsoleParams() {
    db::detail::ConsoleHead* console = EGG::Exception::console;
    console->viewLines = 0x15;
    console->viewPosX = 0x5;
    console->viewPosY = 0xA;
}
BootHook ConsoleParams(SetConsoleParams, 1);

ExceptionFile::ExceptionFile(const OS::Context& context) : magic('PULD'), region(*reinterpret_cast<u32*>(OS::BootInfo::mInstance.diskID.gameName)), reserved(-1) {
    this->srr0.name = 'srr0';
    this->srr0.gpr = context.srr0;
    this->srr1.name = 'srr1';
    this->srr1.gpr = context.srr1;
    this->msr.name = 'msr:';
    this->msr.gpr = PPCMfmsr();
    this->cr.name = 'cr: ';
    this->cr.gpr = context.cr;
    this->lr.name = 'lr: ';
    this->lr.gpr = context.lr;
    for (u32 i = 0; i < 32; ++i) {
        this->gprs[i].gpr = context.gpr[i];  // r00: //r01:
        this->fprs[i].fpr = context.fpr[i];
        const u32 tens = i / 10U;
        const u32 units = i - tens * 10U;
        const u32 regValue = units * 0x100U + tens * 0x10000U;
        this->gprs[i].name = 'r00:' + regValue;
        this->fprs[i].name = 'f00:' + regValue;
    }
    this->fpscr.name = 'fscr';
    this->fpscr.fpr = context.fpscr;
    u32* sp = (u32*)context.gpr[1];
    for (int i = 0; i < 10; ++i) {
        if (sp == nullptr || (u32)sp == 0xFFFFFFFF) break;
        this->frames[i].sp = (u32)sp;
        this->frames[i].lr = sp[1];
        sp = (u32*)*sp;
    }
    
    // Initialize SillyKartWii specific data
    this->vanillaMode = 0;
    this->sillyMode = 0;
    this->sectionMode = 0;
    this->currentPage = 0;
    this->lastPlayedTrack = 0;
    this->sillyEffects = 0;
    this->sillySounds = 0;
    this->sillyPhysics = 0;
    this->braikeDrifting = 0;
    this->freeRoamMode = 0;
}

// Function to update crash data before each page load
void UpdateCrashData() {
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    if (cupsConfig != nullptr) {
        PulsarId pulsarId = cupsConfig->GetWinning();
        LastPlayedTrack = CupsConfig::ConvertTrack_PulsarIdToRealId(pulsarId);
    }
    
    const Racedata* racedata = Racedata::sInstance;
    if (racedata != nullptr) {
        VanillaMode = racedata->menusScenario.settings.gamemode;
    }
    
    SectionMode = SectionMgr::sInstance->curSection->sectionId;
    
    // Get SillyKartWii specific settings
    SillyMode = U8_SILLY_MODE;
    SillyEffects = U16_SILLY_EFFECTS;
    SillySounds = U16_SILLY_SOUNDS;
    SillyPhysics = U8_SILLY_PHYSICS;
    BraikeDrifting = U8_BRAKEDRIFTING;
    FreeRoamMode = U16_FREE_ROAM;
}
static PageLoadHook2 UPDATE_CRASH_DATA(UpdateCrashData);

static void WriteHeaderCrash(u16 error, const OS::Context* context, u32 dsisr, u32 dar) {
    crashError = error;
    crashThread = const_cast<OS::Thread*>(reinterpret_cast<const OS::Thread*>(context));
    db::ExceptionHead& exception = db::ExceptionHead::mInstance;
    exception.displayedInfo = 0x23;
    exception.callbackArgs = nullptr;

    db::Exception_Printf_("=========================================================\n");
    db::Exception_Printf_(" Damn yo shi just got SILLY! get it? nah? ok\n");
    db::Exception_Printf_("=========================================================\n\n");
    db::Exception_Printf_("anyways, SKWii just crashed.\n");
    db::Exception_Printf_("If this is the first time, try to\nreplicate it. If it happened multiple\ntimes, please report this crash.\n\n");
    db::Exception_Printf_("Discord: discord.gg/j3MeZAkYPn\n");
    db::Exception_Printf_("Crash.pul will be saved automatically.\n");
    db::Exception_Printf_("You can find it in SKWCrash/Crash.pul\n\n");
    
    db::Exception_Printf_("--- Crash Information ---\n");
    db::Exception_Printf_("Version: SKWii v2.0.0\n");
    db::Exception_Printf_("Vanilla Mode: %d\n", VanillaMode);
    db::Exception_Printf_("Silly Mode: %d\n", SillyMode);
    db::Exception_Printf_("Silly Physics: %d\n", SillyPhysics);
    db::Exception_Printf_("Brake Drifting: %d\n", BraikeDrifting);
    db::Exception_Printf_("Section: 0x%X\n", SectionMode);
    
    if (REGION == 'P') db::Exception_Printf_("Region: PAL\n");
    else if (REGION == 'E') db::Exception_Printf_("Region: NTSC-U\n");
    else if (REGION == 'J') db::Exception_Printf_("Region: NTSC-J\n");
    else if (REGION == 'K') db::Exception_Printf_("Region: NTSC-K\n");
    
    db::Exception_Printf_("Last Track: %d\n", LastPlayedTrack);
    db::Exception_Printf_("Effects: 0x%04X | Sounds: 0x%04X\n", SillyEffects, SillySounds);
    db::Exception_Printf_("Free Roam: 0x%04X\n\n", FreeRoamMode);
    
    db::PrintContext_(error, context, dsisr, dar);
}
kmCall(0x80023484, WriteHeaderCrash);


static void CreateCrashFile(s32 channel, KPAD::Status buff[], u32 count) {
    IO* io = IO::sInstance;
    
    // Try to save the crash file
    bool savedSuccessfully = false;
    
    if (io != nullptr) {
        OS::Thread* thread = crashThread;
        if (thread != nullptr) {
            OS::DetachThread(thread);
            OS::CancelThread(thread);
            alignas(0x20) ExceptionFile exception(thread->context);
            exception.error = static_cast<OS::Error>(crashError);
            
            // Fill in SillyKartWii specific crash data
            exception.vanillaMode = VanillaMode;
            exception.sillyMode = SillyMode;
            exception.sectionMode = SectionMode;
            exception.currentPage = *reinterpret_cast<u8*>(0x809C20F8);
            exception.lastPlayedTrack = LastPlayedTrack;
            exception.sillyEffects = SillyEffects;
            exception.sillySounds = SillySounds;
            exception.sillyPhysics = SillyPhysics;
            exception.braikeDrifting = BraikeDrifting;
            exception.freeRoamMode = FreeRoamMode;
            
            char path[IOS::ipcMaxPath];
            const System* system = System::sInstance;
            
            if (system != nullptr) {
                snprintf(path, IOS::ipcMaxPath, "%s/Crash.pul", system->GetModFolder());
            } else {
                // Fallback path if System is not initialized
                snprintf(path, IOS::ipcMaxPath, "/SillyKartWii/Crash.pul");
            }
            
            db::Exception_Printf_("\n>>> Saving crash to: %s\n", path);
            
            bool created = io->CreateAndOpen(path, IOS::MODE_READ_WRITE);
            if (created) {
                s32 written = io->Overwrite(sizeof(ExceptionFile), &exception);
                io->Close();
                db::Exception_Printf_(">>> Crash.pul saved successfully! (%d bytes)\n", written);
                db::Exception_Printf_(">>> Please share this file with the developers.\n\n");
                savedSuccessfully = true;
            } else {
                db::Exception_Printf_(">>> ERROR: Failed to create Crash.pul via IO!\n");
            }
            
            // Also write the crash EPH file for the channel
            NewChannel_WriteCrashEphFile();
        }
    } else {
        // IO system not available - try direct ISFS write as fallback
        db::Exception_Printf_("\n>>> IO system not initialized, trying ISFS...\n");
        
        OS::Thread* thread = crashThread;
        if (thread != nullptr) {
            OS::DetachThread(thread);
            OS::CancelThread(thread);
            alignas(0x20) ExceptionFile exception(thread->context);
            exception.error = static_cast<OS::Error>(crashError);
            
            // Fill in SillyKartWii specific crash data
            exception.vanillaMode = VanillaMode;
            exception.sillyMode = SillyMode;
            exception.sectionMode = SectionMode;
            exception.currentPage = 0;
            exception.lastPlayedTrack = LastPlayedTrack;
            exception.sillyEffects = SillyEffects;
            exception.sillySounds = SillySounds;
            exception.sillyPhysics = SillyPhysics;
            exception.braikeDrifting = BraikeDrifting;
            exception.freeRoamMode = FreeRoamMode;
            
            // Try to save to NAND as fallback - use shared2 for persistence
            char path[IOS::ipcMaxPath];
            
            // First try to create the directory
            ISFS::CreateDir("/shared2", 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            ISFS::CreateDir("/shared2/SKWCrash", 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            
            snprintf(path, IOS::ipcMaxPath, "/shared2/SKWCrash/Crash.pul");
            
            db::Exception_Printf_(">>> Attempting NAND write to: %s\n", path);
            
            // Create file with ISFS
            s32 result = ISFS::CreateFile(path, 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            if (result >= 0 || result == -105) { // -105 = already exists
                s32 fd = ISFS::Open(path, ISFS::MODE_READ_WRITE);
                if (fd >= 0) {
                    s32 written = ISFS::Write(fd, &exception, sizeof(ExceptionFile));
                    ISFS::Close(fd);
                    
                    if (written == sizeof(ExceptionFile)) {
                        db::Exception_Printf_(">>> Crash saved to NAND! (%d bytes)\n", written);
                        db::Exception_Printf_(">>> File location: %s\n", path);
                        savedSuccessfully = true;
                    } else {
                        db::Exception_Printf_(">>> ERROR: ISFS Write failed (%d)\n", written);
                    }
                } else {
                    db::Exception_Printf_(">>> ERROR: ISFS Open failed (%d)\n", fd);
                }
            } else {
                db::Exception_Printf_(">>> ERROR: ISFS CreateFile failed (%d)\n", result);
            }
        }
    }
    
    if (!savedSuccessfully) {
        db::Exception_Printf_("\n>>> WARNING: Could not save crash file!\n");
        db::Exception_Printf_(">>> Please take a photo of this screen.\n");
    }
    
    // Now wait for A button before exiting
    db::Exception_Printf_("\nPress A to exit...\n");
    
    bool exit = false;
    while (!exit) {
        KPAD::Read(channel, buff, count);
        u8 wiimoteExtension = buff[0].extension;
        
        // Check Wiimote/Classic Controller A button
        if (buff[0].error == WPAD::WPAD_ERR_NONE && wiimoteExtension < WPAD::WPAD_DEV_FUTURE &&
            (wiimoteExtension == WPAD::WPAD_DEV_CLASSIC && buff[0].extStatus.cl.trig & WPAD::WPAD_CL_BUTTON_A || 
             buff[0].trig & WPAD::WPAD_BUTTON_A)) {
            exit = true;
            break;
        }
        
        // Check GameCube Controller A button
        PAD::Status padStatus[4];
        PAD::Read(&padStatus[0]);
        for (int i = 0; i < 4; ++i) {
            if (padStatus[i].error == PAD::PAD_ERR_NONE) {
                if (padStatus[i].buttons & PAD::PAD_BUTTON_A) {
                    exit = true;
                    break;
                }
            }
        }
        
        // Yield to prevent busy-waiting
        OSYieldThread();
    }
    
    LaunchSoftware();
}
kmCall(0x80226610, CreateCrashFile);

}  // namespace Debug
}  // namespace Pulsar
