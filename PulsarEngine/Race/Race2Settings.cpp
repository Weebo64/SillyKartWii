#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace Race {

// Trick Chaining [RoGamer97]
// Triple Bananas Rotate [IKW]
// Turn in Air [IKW]

kmRuntimeUse(0x808a5380);
kmRuntimeUse(0x8057A9F8);
kmRuntimeUse(0x8057C9C8);
kmRuntimeUse(0x80575b60);
kmRuntimeUse(0x80575bd4);
kmRuntimeUse(0x8057a634);
kmRuntimeUse(0x80575b54);
kmRuntimeUse(0x80575b94);

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
        beq loc_end;
        lbz r12, 0xDB(r31);
        addi r12, r12, 1;
        cmpwi r12, 30;
        bgt skip1;
        stb r12, 0xDB(r31);
    skip1:
        mr r5, r6;
        cmpwi r12, 20;
        bgt skip2;
        li r5, 0;
    skip2:
        cmpwi r5, 0;
        beq loc_end;
        li r12, 1;
        stw r12, 0x74(r4);
        lwz r12, 0x4(r3);
        lwz r12, 0x8(r12);
        andi. r12, r12, 0x400;
        bne skip3;
        li r12, 5;
        stw r12, 0x1C(r4);
    skip3:
        li r12, 0;
        stb r12, 0xDB(r31);
    loc_end:
        blr;
    )
}

void ApplyRace2Settings() {
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x808a5380)) = 0x02;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TRIPLE_BANANAS) == RACE2SETTING_TRIPLE_BANANAS_ROTATE) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x808a5380)) = 0x03;
    }

    *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057A9F8)) = 0x41820014;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057C9C8)) = 0x4082017C;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TURN_IN_AIR) == RACE2SETTING_TURN_IN_AIR_ENABLED) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057A9F8)) = 0x48000014;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057C9C8)) = 0x4800017C;
    }

    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b60)) = 0x70000001;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575bd4)) = 0x4800000C;
    *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057a634)) = 0x60000000;
    kmRuntimeCallA(0x80575b54, TrickChaining1);
    kmRuntimeCallA(0x80575b94, TrickChaining2);
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_TRICK_CHAINING) == RACE2SETTING_TRICK_CHAINING_DISABLED) {
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b60)) = 0x88A3003A;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575bd4)) = 0x80A4001C;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x8057a634)) = 0x80A4001C;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b54)) = 0x88A3003A;
        *reinterpret_cast<u32*>(kmRuntimeAddr(0x80575b94)) = 0x80A4001C;
    }
}

static RaceLoadHook ApplyRace2SettingsHook(ApplyRace2Settings);

}//namespace Race
}//namespace Pulsar
