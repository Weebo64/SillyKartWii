#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Input/ControllerHolder.hpp>
#include <MarioKartWii/Kart/KartStatus.hpp>
#include <MarioKartWii/Kart/KartMovement.hpp>
#include <MarioKartWii/Effect/PlayerEffects.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>

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
