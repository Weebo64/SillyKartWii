#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Gamemodes/RealMarioKart/RealMarioKartMgr.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>

// Note: this is not finished/untested. feel free to modify the code to help me out! -Weebo64

kmRuntimeUse(0x80558F90);

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

// Apply No Vehicles when the gamemode is active n shi
void ApplyRealMarioKartSettings() {
    if(Mgr::IsActive()) {
        // Disable vehicle rendering (No Vehicles code from JoshuaMK)
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80558F90)) = 0x60000000;
    }
}

static RaceLoadHook ApplyRealMarioKartHook(ApplyRealMarioKartSettings);

} // namespace RealMarioKart
} // namespace Pulsar
