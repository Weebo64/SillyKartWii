#include <kamek.hpp>
#include <MarioKartWii/Race/Racedata.hpp>
#include <MarioKartWii/Race/Raceinfo/Raceinfo.hpp>
#include <MarioKartWii/Kart/KartManager.hpp>
#include <MarioKartWii/Objects/ObjectsMgr.hpp>
#include <MarioKartWii/KMP/KMPManager.hpp>
#include <MarioKartWii/3D/Camera/CameraMgr.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <MarioKartWii/Input/InputManager.hpp>
#include <PulsarSystem.hpp>
#include <Gamemodes/FreeRoam/FRMgr.hpp>
#include <Settings/SettingsParam.hpp>
#include <Settings/Settings.hpp>
#include <RuntimeWrite.hpp>
#include <SillyKartWii.hpp>

// Free Roam Gamemode by Toadette Hack Fan

namespace Pulsar {
namespace FreeRoam {

Mgr* Mgr::sInstance = nullptr;

Mgr::Mgr() {
}

Mgr::~Mgr() {
}

void Mgr::Create() {
    if(sInstance == nullptr) {
        sInstance = new Mgr();
    }
}

void Mgr::Update() {
    if(sInstance != nullptr && System::sInstance->IsContext(PULSAR_MODE_IKW)) {
    }
}

void UpdateInputs() {
    const Input::Manager* inputMgr = Input::Manager::sInstance;
    if(inputMgr != nullptr) {
        const Input::RealControllerHolder* holder = &inputMgr->realControllerHolders[0];
        if(holder != nullptr && holder->curController != nullptr) {
            const Input::Controller* controller = holder->curController;
            ControllerType type = controller->GetType();
            
            if(type == GCN) {
                const Input::GCNController* gcn = static_cast<const Input::GCNController*>(controller);
                GCInput = gcn->buttonRaw;
            }
            else if(type == WHEEL || type == CLASSIC || type == NUNCHUCK) {
                const Input::WiiController* wii = static_cast<const Input::WiiController*>(controller);
                WiiInput = wii->kpadStatus[0].hold;
                ClassicInput = wii->clButtons;
            }
        }
    }
}
static RaceFrameHook UpdateInputsHook(UpdateInputs);

asmFunc Mushrooms() {
    ASM(
  li        r12, 0x1;
  stw       r12, 0x90(r4);
  li        r12, 0x4;
  stw       r12, 0x8C(r4);
  lwz       r0, 0x8C(r4);
  blr;
    )
}

asmFunc Feather() {
    ASM(
  li        r12, 0x1;
  stw       r12, 0x90(r4);
  li        r12, 0xC;
  stw       r12, 0x8C(r4);
  lwz       r0, 0x8C(r4);
  blr;
    )
}

asmFunc AutoDrive1() {
    ASM(
  lwz       r0, 0x18(r30);
  cmpwi     r0, 0;
  bne-      loc_0x10;
  li        r0, 0x1;

loc_0x10:
  blr;
    )
}

asmFunc AutoDrive2() {
    ASM(
  lwz       r3, 0x38(r3);
  cmpwi     r3, 0;
  bne-      loc_0x10;
  li        r3, 0x1;

loc_0x10:
  blr;
    )
}

kmRuntimeUse(0x805349d4);
kmRuntimeUse(0x8053353c);
kmRuntimeUse(0x805a7408);
kmRuntimeUse(0x80633bb0);
kmRuntimeUse(0x8069b40c);
kmRuntimeUse(0x80716638);
kmRuntimeUse(0x80725afc);
kmRuntimeUse(0x80857768);
kmRuntimeUse(0x808577e0);
kmRuntimeUse(0x808409fe);
kmRuntimeUse(0x80840a00);
kmRuntimeUse(0x80571714);
kmRuntimeUse(0x80571dc4);
kmRuntimeUse(0x805867cc);
kmRuntimeUse(0x805954fc);
kmRuntimeUse(0x808b5b1c);
kmRuntimeUse(0x807eee98);
kmRuntimeUse(0x8052F538);
kmRuntimeUse(0x80726270);
void FreeRoam() {
    kmRuntimeWrite32A(0x805349d4, 0x80A30038);
    kmRuntimeWrite32A(0x8053353c, 0x801F0020);
    kmRuntimeWrite32A(0x805a7408, 0x807D0078);
    kmRuntimeWrite32A(0x80633bb0, 0x3860116E);
    if(REGION == 'P') kmRuntimeWrite32A(0x8069b40c, 0x4BE97C85);
    if(REGION == 'E') kmRuntimeWrite32A(0x8069b40c, 0x4BE975C5);
    if(REGION == 'J') kmRuntimeWrite32A(0x8069b40c, 0x4BE97F99);
    if(REGION == 'K') kmRuntimeWrite32A(0x8069b40c, 0x4BE97935);
    kmRuntimeWrite32A(0x80716638, 0x280400D6);
    kmRuntimeWrite32A(0x80725afc, 0x88030006);
    kmRuntimeWrite32A(0x80857768, 0x40820090);
    kmRuntimeWrite32A(0x808577e0, 0x388000D6);
    kmRuntimeWrite16A(0x808409fe, 0x0070);
    if(REGION != 'E') kmRuntimeWrite32A(0x80840a00, 0x4BFF6D21);
    else kmRuntimeWrite32A(0x80840a00, 0x4BFF9889);
    kmRuntimeWrite16A(0x80571714, 0x4180);
    kmRuntimeWrite16A(0x80571dc4, 0x4182);
    kmRuntimeWrite16A(0x805867cc, 0x4182);
    kmRuntimeWrite32A(0x807eee98, 0x8004008C);
    kmRuntimeWrite32A(0x8052F538, 0x801E0018);
    kmRuntimeWrite32A(0x80726270, 0x80630038);
    if(U16_FREE_ROAM == 0x1) {
        const Settings::Mgr& settingsMgr = Settings::Mgr::Get();
        
        u8 fallBoundaries = settingsMgr.GetSettingValue(Settings::SETTINGSTYPE_FREEROAM, SETTINGFREEROAM_RADIO_FALL_BOUNDARIES);
        u8 startItem = settingsMgr.GetSettingValue(Settings::SETTINGSTYPE_FREEROAM, SETTINGFREEROAM_RADIO_START_ITEM);
        u8 autoDrive = settingsMgr.GetSettingValue(Settings::SETTINGSTYPE_FREEROAM, SETTINGFREEROAM_RADIO_AUTO_DRIVE);
        u8 respawnButton = settingsMgr.GetSettingValue(Settings::SETTINGSTYPE_FREEROAM, SETTINGFREEROAM_RADIO_RESPAWN_BUTTON);
        
        kmRuntimeWrite32A(0x805349d4, 0x38A00002);
        kmRuntimeWrite32A(0x8053353c, 0x380000F0);
        kmRuntimeWrite32A(0x805a7408, 0x386000AA);
        kmRuntimeWrite32A(0x80633bb0, 0x38600048);
        kmRuntimeWrite32A(0x8069b40c, 0x38600000);
        kmRuntimeWrite32A(0x80725afc, 0x38000001);
        kmRuntimeWrite32A(0x80857768, 0x60000000);
        kmRuntimeWrite32A(0x808b5b1c, 0x00000000);
        kmRuntimeWrite16A(0x808409fe, 0x001F);
        kmRuntimeWrite32A(0x80840a00, 0x42800015);
        kmRuntimeWrite16A(0x80571714, 0x4800);
        kmRuntimeWrite16A(0x80571dc4, 0x4800);
        kmRuntimeWrite16A(0x805867cc, 0x4800);
        
        kmRuntimeWrite32A(0x80716638, 0x280401CF);
        kmRuntimeWrite32A(0x808577e0, 0x388001CF);
        
        if(fallBoundaries == FREEROAMSETTING_FALL_BOUNDARIES_DISABLED) {
            kmRuntimeWrite16A(0x80571714, 0x4800);
            kmRuntimeWrite16A(0x80571dc4, 0x4800);
            kmRuntimeWrite16A(0x805867cc, 0x4800);
        }
        
        if(startItem == FREEROAMSETTING_START_ITEM_MUSHROOM) {
            kmRuntimeCallA(0x807eee98, Mushrooms);
        }
        else if(startItem == FREEROAMSETTING_START_ITEM_FEATHER) {
            kmRuntimeCallA(0x807eee98, Feather);
        }
        
        if(autoDrive == FREEROAMSETTING_AUTO_DRIVE_ENABLED) {
            kmRuntimeCallA(0x8052F538, AutoDrive1);
            kmRuntimeCallA(0x80726270, AutoDrive2);
        }
    }
}
static PageLoadHook FREEROAM(FreeRoam);

void FreeRoamRespawn() {
    kmRuntimeWrite32A(0x805954fc, 0x80B40004);
    if(U16_FREE_ROAM == 0x1) {
        if(GCInput == 0x0880 || ClassicInput == 0x1000 || WiiInput == 0x1000) kmRuntimeWrite32A(0x805954fc, 0x38A00010);
    }
}
static PageLoadHook2 FREEROAMRESPAWN(FreeRoamRespawn);

}
}
