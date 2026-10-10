#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Gamemodes/RealMarioKart/RealMarioKartMgr.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>

// Note: this is not finished/untested. feel free to modify the code to help me out! -Weebo64

// No Vehicles addresses by region
// PAL: 0x8055D310, original: 0x408200CC
// NTSC-U: 0x80558F90, original: 0x408200CC
// NTSC-J: 0x8055CC90, original: 0x408200CC
// NTSC-K: 0x8054B368, original: 0x408200CC

namespace Pulsar {
namespace RealMarioKart {

Mgr::Mgr() {
}

Mgr::~Mgr() {
}

void Mgr::Create() {
    System* system = System::sInstance;
    if(system->realMarioKartMgr == nullptr) {
        system->realMarioKartMgr = new Mgr();
    }
}

bool Mgr::IsActive() {
    const System* system = System::sInstance;
    if(system == nullptr) return false;
    
    // Check if we're in Friend Room or not
    const RKNet::Controller* controller = RKNet::Controller::sInstance;
    if(controller == nullptr) return false;
    
    const RKNet::RoomType roomType = controller->roomType;
    const bool isInFriendRoom = (roomType == RKNet::ROOMTYPE_FROOM_HOST || roomType == RKNet::ROOMTYPE_FROOM_NONHOST);
    
    // Check if the gamemode is enabled via context (set by host through network packet)
    if(system->realMarioKartMgr != nullptr && isInFriendRoom) {
        return system->IsContext(PULSAR_MODE_REALMARIOKART);
    }
    
    return false;
}

// NOTE: Runtime patching causes crashes
// We need a different approach - perhaps using code injection hooks
// instead of directly modifying instructions
// For now, this feature is disabled until we find a stable solution

} // namespace RealMarioKart
} // namespace Pulsar
