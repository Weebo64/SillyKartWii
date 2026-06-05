#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Input/ControllerHolder.hpp>
#include <MarioKartWii/Kart/KartStatus.hpp>
#include <MarioKartWii/Kart/KartMovement.hpp>
#include <MarioKartWii/Effect/PlayerEffects.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>

namespace Pulsar {
namespace Race {

extern "C" void sInstance__8Racedata(void*);

// Trick Chaining [RoGamer97]
// Triple Bananas Rotate [IKW]
// Turn in Air [IKW]
// Brake Drifting [IKW]
// No Vehicles [JoshuaMK] - Host Only
// Allow Looking Backwards Anytime [Ro, Gaberboo]
// No Lightning Flash [Ro]
// Mii Nametag Color [Conradi]
// Items on minimap (from IKW Team)

kmRuntimeUse(0x808a5380);
kmRuntimeUse(0x8057A9F8);
kmRuntimeUse(0x8057C9C8);
kmRuntimeUse(0x80575b64);
kmRuntimeUse(0x80575bd4);
kmRuntimeUse(0x8057a634);
kmRuntimeUse(0x80575b54);
kmRuntimeUse(0x80575b94);
kmRuntimeUse(0x80558F90);
kmRuntimeUse(0x8054F1B0);
kmRuntimeUse(0x805A228C);
kmRuntimeUse(0x805A225C);
kmRuntimeUse(0x807F042C);
kmRuntimeUse(0x80858194);
kmRuntimeUse(0x807ea450);
kmRuntimeUse(0x807ea6e0);
kmRuntimeUse(0x807eaca8);

static float brakeDriftingDeceleration = -3.0f;
u8 brakeDriftingEnabled = 0;

bool IsInFriendRoom() {
    const RKNet::Controller* controller = RKNet::Controller::sInstance;
    if(controller == nullptr) return false;
    const RKNet::RoomType roomType = controller->roomType;
    return (roomType == RKNet::ROOMTYPE_FROOM_HOST || roomType == RKNet::ROOMTYPE_FROOM_NONHOST);
}

asmFunc TrickChaining1() {
    ASM(
        nofralloc;
        lbz r5, 0x3A(r3);
        mr r6, r5;
        blr;
    )
}

asmFunc TrickChaining2() {
    ASM(
        nofralloc;
        lwz r5, 0x1C(r4);
        lwz r11, 0x4(r3);
        lwz r11, 0x8(r11);
        andi. r0, r11, 0x8040;
        beq loc_0x60;
        lbz r12, 0xDB(r31);
        addi r12, r12, 0x1;
        cmpwi r12, 0x1E;
        bge loc_0x28;
        stb r12, 0xDB(r31);
    loc_0x28:
        mr r5, r6;
        cmpwi r12, 0x14;
        bge loc_0x38;
        li r5, 0;
    loc_0x38:
        cmpwi r5, 0;
        beq loc_0x68;
        li r12, 0x1;
        stw r12, 0x74(r4);
        lwz r12, 0x4(r3);
        lwz r12, 0x8(r12);
        andi. r12, r12, 0x400;
        bne loc_0x60;
        li r12, 0x5;
        stw r12, 0x1C(r4);
    loc_0x60:
        li r12, 0;
        stb r12, 0xDB(r31);
    loc_0x68:
        blr;
    )
}

extern "C" void sInstance__Q24Item7Manager(void*);
asmFunc ItemsOnMinimap1() {
  ASM(
    nofralloc;
  lis       r3, sInstance__Q24Item7Manager@ha;
  lwz       r3, sInstance__Q24Item7Manager@l(r3);
  lwz       r4, 0x244(r3);
  lwz       r4, 0x0(r4);
  lhz       r4, 0x8(r4);
  lwz       r3, 0x248(r3);
  add       r3, r4, r3;
  mulli     r3, r3, 0x1B4;
  addi      r3, r3, 0x1A4;
  blr;
  )
}

asmFunc ItemsOnMinimap2() {
  ASM(
    nofralloc;
  addi      r26, r27, 0x1;
  lis       r5, sInstance__Q24Item7Manager@ha;
  lwz       r5, sInstance__Q24Item7Manager@l(r5);
  lwz       r6, 0x244(r5);
  lwz       r6, 0x0(r6);
  lhz       r6, 0x8(r6);
  lwz       r5, 0x248(r5);
  add       r5, r6, r5;
  add       r26, r26, r5;
  blr;
  )
}

extern "C" void ItemsOnMinimapEnd3(void*);
extern "C" void __ct__15LayoutUIControlFv(void*);
extern "C" void ItemsOnMinimapSymbol1(void*);
extern "C" void AddControl__9UIControlFUiP9UIControl(void*);
extern "C" void ItemsOnMinimapSymbol2(void*);
extern "C" void ItemsOnMinimapSymbol3(void*);
extern "C" void ItemsOnMinimapSymbol4(void*);
extern "C" void Load__13ControlLoaderFPCcPCcPCcPPCc(void*);
extern "C" void ItemsOnMinimapSymbol5(void*);
extern "C" void ItemsOnMinimapSymbol6(void*);
extern "C" void ItemsOnMinimapSymbol7(void*);
extern "C" void ItemsOnMinimapSymbol9(void*);
extern "C" void SetPicturePane__15LayoutUIControlFPCcPCc(void*);
asmFunc ItemsOnMinimap3() {
  ASM(
    nofralloc;
  bctrl;
  li        r26, 0;
  addi      r23, r24, 0x1A4;

loc_0xC:
  li        r25, 0;

loc_0x10:
  mr        r3, r23;
  lis       r12, __ct__15LayoutUIControlFv@h;
  ori       r12, r12, __ct__15LayoutUIControlFv@l;
  mtctr     r12;
  bctrl;     
  lis       r3, ItemsOnMinimapSymbol1@h;
  ori       r3, r3, ItemsOnMinimapSymbol1@l;
  stw       r3, 0x0(r23);
  mr        r3, r24;
  addi      r28, r28, 0x1;
  mr        r4, r28;
  mr        r5, r23;
  lis       r12, AddControl__9UIControlFUiP9UIControl@h;
  ori       r12, r12, AddControl__9UIControlFUiP9UIControl@l;
  mtctr     r12;
  bctrl;     
  addi      r3, r1, 0x20;
  stw       r23, 0x0(r3);
  lis       r6, ItemsOnMinimapSymbol2@h;
  ori       r4, r6, ItemsOnMinimapSymbol2@l;
  ori       r5, r6, ItemsOnMinimapSymbol3@l;
  ori       r6, r6, ItemsOnMinimapSymbol4@l;
  li        r7, 0;
  lis       r12, Load__13ControlLoaderFPCcPCcPCcPPCc@h;
  ori       r12, r12, Load__13ControlLoaderFPCcPCcPCcPPCc@l;
  mtctr     r12;
  bctrl;     
  mr        r3, r23;
  lis       r5, ItemsOnMinimapSymbol2@h;
  ori       r4, r5, ItemsOnMinimapSymbol2@l;
  ori       r5, r5, ItemsOnMinimapSymbol5@l;
  lis       r12, ItemsOnMinimapSymbol6@h;
  ori       r12, r12, ItemsOnMinimapSymbol6@l;
  mtctr     r12;
  bctrl;     
  stb       r26, 0x81(r23);
  stb       r25, 0x82(r23);
  lis       r6, ItemsOnMinimapSymbol7@h;
  ori       r3, r6, ItemsOnMinimapSymbol7@l;
  bl        loc_0xC0;
  opword    0x000c182c;
  opword    0x604c5820;
  opword    0x7840848c;
  opword    0x689c9000;

loc_0xC0:
  mflr      r4;
  lbzx      r4, r4, r26;
  add       r5, r3, r4;
  mr        r3, r23;
  ori       r4, r6, ItemsOnMinimapSymbol9@l;
  lis       r12, SetPicturePane__15LayoutUIControlFPCcPCc@h;
  ori       r12, r12, SetPicturePane__15LayoutUIControlFPCcPCc@l;
  mtctr     r12;
  bctrl;     
  addi      r23, r23, 0x1B4;
  addi      r25, r25, 0x1;
  lis       r3, sInstance__Q24Item7Manager@ha;
  lwz       r3, sInstance__Q24Item7Manager@l(r3);
  mulli     r4, r26, 0x24;
  add       r3, r3, r4;
  lwz       r3, 0x50(r3);
  cmpw      r25, r3;
  blt+      loc_0x10;
  addi      r26, r26, 0x1;
  cmpwi     r26, 0xF;
  blt+      loc_0xC;
  b ItemsOnMinimapEnd3;
  )
}

extern "C" void ItemsOnMinimapEnd4(void*);
asmFunc ItemsOnMinimap4() {
  ASM(
    nofralloc;
  bctrl;     
  lwz       r3, 0x16C(r31);
  cmpwi     r3, 0;
  beq-      loc_0x84;
  lwz       r3, 0x198(r31);
  li        r4, 0xA0;
  stb       r4, 0xB8(r3);
  lis       r4, 0x4140;
  stw       r4, 0x4C(r3);
  stw       r4, 0x50(r3);
  li        r3, 0x1;
  stb       r3, 0x80(r31);
  lis       r3, sInstance__Q24Item7Manager@ha;
  lwz       r3, sInstance__Q24Item7Manager@l(r3);
  lbz       r4, 0x81(r31);
  mulli     r4, r4, 0x24;
  add       r3, r3, r4;
  lwz       r4, 0x58(r3);
  lbz       r5, 0x81(r31);
  cmpwi     r5, 0x7;
  beq-      loc_0x84;
  lbz       r5, 0x82(r31);
  cmpw      r5, r4;
  bge-      loc_0x84;
  li        r4, 0;
  stb       r4, 0x80(r31);
  lwz       r3, 0x4C(r3);
  rlwinm    r4,r5,2,0,29;
  lwzx      r3, r3, r4;
  addi      r4, r3, 0x44;
  lswi      r5, r4, 0xC;
  addi      r4, r31, 0x19C;
  stswi     r5, r4, 0xC;

loc_0x84:
  b ItemsOnMinimapEnd4;
  )
}

extern "C" void sInstance__8Racedata(void*);
asmFunc MiiNametagColor() {
    ASM(
        nofralloc;
        lis r12, sInstance__8Racedata@ha;
        lwz r12, sInstance__8Racedata@l(r12);
        mulli r11, r30, 0xF0;
        addi r11, r11, 0x28;
        add r12, r12, r11;
        lwz r11, 0x74(r12);
        stw r11, 0x14(r1);
        stw r11, 0x2C(r1);
        lwz r28, 0x2C(r1);
        blr;
    )
}

void ApplyRace2Settings() {
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x808a5380)) = 0x02;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TRIPLE_BANANAS) == RACE2SETTING_TRIPLE_BANANAS_ROTATE) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x808a5380)) = 0x03;
    }

    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b64)) = 0x60000000;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575bd4)) = 0x4800000C;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057a634)) = 0x60000000;
    kmRuntimeCallA(0x80575b54, TrickChaining1);
    kmRuntimeCallA(0x80575b94, TrickChaining2);
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TRICK_CHAINING) == RACE2SETTING_TRICK_CHAINING_DISABLED) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b64)) = 0x40820028;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575bd4)) = 0x40820038;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057a634)) = 0x40820058;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b54)) = 0x88A3003A;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b94)) = 0x80A4001C;
    }
    
    brakeDriftingEnabled = 0x00;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_BRAKE_DRIFTING) == RACE2SETTING_BRAKE_DRIFTING_ENABLED) {
        brakeDriftingEnabled = 0x01;
    }
    
    // No Vehicles (Host Only - Friend Rooms)
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80558F90)) = 0x408200CC;
    if(IsInFriendRoom() && Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_HOST, SETTINGHOST_RADIO_NO_VEHICLES) == HOSTSETTING_NO_VEHICLES_ENABLED) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80558F90)) = 0x60000000;
    }
    
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x8054F1B0)) = 0x9421FFD0;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_RADIO_NO_LIGHTNING_FLASH) == MISCSETTING_NO_LIGHTNING_FLASH_ENABLED) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x8054F1B0)) = 0x4E800020;
    }
    
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x805A228C)) = 0x60000000; // Respawn
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x805A225C)) = 0x38800001; // Countdown
    
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x807F042C)) = 0x8381002C;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_RADIO_MII_NAMETAG) == MISCSETTING_MII_NAMETAG_MII_COLOR) {
        kmRuntimeCallA(0x807F042C, MiiNametagColor);
    }
    
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80858194)) = 0x386001A4;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x807ea450)) = 0x3B5B0001;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x807ea6e0)) = 0x4E800421;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x807eaca8)) = 0x4E800421;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_RADIO_ITEMS_ON_MINIMAP) == MISCSETTING_ITEMS_ON_MINIMAP_ENABLED) {
        kmRuntimeCallA(0x80858194, ItemsOnMinimap1);
        kmRuntimeCallA(0x807ea450, ItemsOnMinimap2);
        kmRuntimeBranchA(0x807ea6e0, ItemsOnMinimap3);
        kmRuntimeBranchA(0x807eaca8, ItemsOnMinimap4);
    }
}

void ApplyTurnInAir() {
    kmRuntimeWrite32A(0x8057A9F8, 0x41820014);
    kmRuntimeWrite32A(0x8057C9C8, 0x4082017C);
    if(System::sInstance->IsContext(PULSAR_200)) {
        if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TURN_IN_AIR) == RACE2SETTING_TURN_IN_AIR_ENABLED) {
            kmRuntimeWrite32A(0x8057A9F8, 0x48000014);
            kmRuntimeWrite32A(0x8057C9C8, 0x4800017C);
        }
    }
}

static RaceLoadHook ApplyRace2SettingsHook(ApplyRace2Settings);
static PageLoadHook ApplyTurnInAirHook(ApplyTurnInAir);

}//namespace Race
}//namespace Pulsar