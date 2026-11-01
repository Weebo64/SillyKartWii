#include <kamek.hpp>
#include <core/rvl/DWC/DWC.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <Network/WiiLink.hpp>
#include <Network/Network.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>

namespace Pulsar {
namespace Network {
//Region Patch (Leseratte)

static void PatchLoginRegion() {
    WWFC_CUSTOM_REGION = System::sInstance->netMgr.region;
    char path[0x9];
    snprintf(path, 0x9, "%08d", System::sInstance->netMgr.region + 100000);
    for(int i = 0; i < 8; ++i) {
        DWC::loginRegion[i] = path[i];
    }
}
BootHook LoginRegion(PatchLoginRegion, 2);


// PatchRegion(0x8065920c);
// PatchRegion(0x80659260);
// PatchRegion(0x80659724);
// PatchRegion(0x80659778);

int PatchRegion(char* path, u32 len, const char* fmt, const char* mode) {
    const Info& info = System::sInstance->GetInfo();
    return snprintf(path, len, fmt, mode, System::sInstance->netMgr.region);
}
kmCall(0x8065921c, PatchRegion);
kmCall(0x80659270, PatchRegion);
kmCall(0x80659734, PatchRegion);
kmCall(0x80659788, PatchRegion);



static u32 PatchRKNetControllerRegion() {
    return System::sInstance->netMgr.region;
}
kmCall(0x80653640, PatchRKNetControllerRegion);
kmWrite32(0x80653644, 0x7c651b78);
kmCall(0x806536ac, PatchRKNetControllerRegion); //for battle
kmWrite32(0x806536b0, 0x7c661b78);

//kmCall(0x80653700, PatchRKNetControllerRegion); this is for battle, right now it'll store 2 (if pal)/FF



//kmWrite32(0x8065A034, 0x3880008E);
//kmWrite32(0x8065A080, 0x3880008E);

}//namespace Network
}//namespace Pulsar