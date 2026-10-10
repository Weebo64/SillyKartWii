#include <kamek.hpp>
#include <Settings/SettingsParam.hpp>
#include <PulsarSystem.hpp>
#include <SillyKartWii.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <Gamemodes/FreeRoam/FRMgr.hpp>

namespace Pulsar {
namespace UI {

// Trickable Cannons :3
asmFunc TrickableCannons() {
    ASM(
  ori       r0, r0, 0x10;
  oris      r0, r0, 0x4000;
  blr;
    )
}
kmCall(0x80584bb0, TrickableCannons);

void GameModeToggle() {
    if(Racedata::sInstance->menusScenario.settings.gamemode != MODE_TIME_TRIAL) U16_FREE_ROAM = 0x0000;

    SectionId id = SectionMgr::sInstance->nextSectionId;
    if(id == SECTION_SINGLE_P_BT_NEXT_BATTLE) {
        id = SECTION_SINGLE_P_VS_NEXT_RACE;
        SectionMgr::sInstance->nextSectionId = id;
    }
    if(id == SECTION_AWARD_38) {
        id = SECTION_MAIN_MENU_FROM_MENU;
        SectionMgr::sInstance->nextSectionId = id;
    }
    if(Racedata::sInstance->menusScenario.settings.gamemode != MODE_GRAND_PRIX && id == SECTION_GP_INTRO) {
        id = SECTION_VS_RACE_INTRO;
        SectionMgr::sInstance->nextSectionId = id;
    }

    if(Racedata::sInstance->menusScenario.settings.gamemode == MODE_TIME_TRIAL) return;
    if(Racedata::sInstance->menusScenario.settings.gamemode == MODE_MISSION_TOURNAMENT) return;
    
    bool isWWVS = Racedata::sInstance->menusScenario.settings.gamemode == MODE_PUBLIC_VS || Racedata::sInstance->menusScenario.settings.gamemode == MODE_PRIVATE_VS;
    bool isWWBT = Racedata::sInstance->menusScenario.settings.gamemode == MODE_PUBLIC_BATTLE || Racedata::sInstance->menusScenario.settings.gamemode == MODE_PRIVATE_BATTLE;
    if(isWWVS || isWWBT) return;
    System::sInstance->UpdateContext();
}
static PageLoadHook2 GAMEMODE(GameModeToggle);

void BrakeDriftingToggle() {
    U8_BRAKEDRIFTING = 0x00;
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_BRAKE_DRIFTING) == RACE2SETTING_BRAKE_DRIFTING_ENABLED) {
        U8_BRAKEDRIFTING = 0x01;
    }
    
    // Reset ItemRain flag
    U16_ITEMRAIN = 0x0000;
    
    // Set ItemRain flag based on context
    if(System::sInstance) {
        if(System::sInstance->IsContext(PULSAR_ITEMMODERAIN)) {
            U16_ITEMRAIN = 0x0001;
        }
    }
}
static PageLoadHook Codes3(BrakeDriftingToggle);

void RecolorsMenus() {
    // Silly Kart Wii color scheme (haha boobs)
    U8_RED1 = 176;
    U8_GREEN1 = 11;
    U8_BLUE1 = 105;
    U8_ALPHA1= 0xFF;
    U8_RED2 = 176;
    U8_GREEN2 = 11;
    U8_BLUE2 = 105;
    U8_ALPHA2= 0xFF;
    U8_RED3 = 176;
    U8_GREEN3 = 11;
    U8_BLUE3 = 105;
    U8_ALPHA3= 0x70;
    U8_RED4 = 176;
    U8_GREEN4 = 11;
    U8_BLUE4 = 105;
    U8_ALPHA4= 0xFF;
    U8_RED5 = 176;
    U8_GREEN5 = 11;
    U8_BLUE5 = 105;
    U8_ALPHA5= 0xFF;
    U8_RED6 = 176;
    U8_GREEN6 = 11;
    U8_BLUE6 = 105;
    U8_ALPHA6= 0x70;
}
static Settings::Hook RECOLORSS(RecolorsMenus);
BootHook RECOLORS(RecolorsMenus, 7);

// Mega Mushroom FOV UwU
asmFunc MegaFOV() {
    ASM(
  lwz       r4, 0x0(r28);
  lwz       r29, 0x24(r4);
  cmpwi     r29, 0;
  beq-      loc_0x28;
  lwz       r3, 0x4(r4);
  lwz       r3, 0xC(r3);
  rlwinm.   r3,r3,0,16,16;
  beq-      loc_0x28;
  lis       r0, 0x41F0;
  stw       r0, 0x120(r29);

loc_0x28:
    blr;
    )
}
kmCall(0x805793AC, MegaFOV);

extern "C" void sInstance__8Racedata(void*);

asmFunc Crown() {
    ASM(
  lwzx      r4, r4, r0;
  li        r11, 0;
  lbz       r12, 0x20(r4);
  cmpwi     r12, 0x1;
  bne-      loc_0x4C;
  lwz       r12, sInstance__8Racedata@l(r3);
  lwz       r12, 0xB70(r12);
  cmpwi     r12, 0x2;
  blt-      loc_0x48;
  cmpwi     r12, 0x3;
  beq-      loc_0x3C;
  cmpwi     r12, 0x7;
  blt-      loc_0x5C;
  cmpwi     r12, 0x9;
  blt-      loc_0x48;

loc_0x3C:
  lhz       r12, 0x22(r4);
  cmpwi     r12, 0x3;
  blt-      loc_0x4C;

loc_0x48:
  li        r11, 0x1;

loc_0x4C:
  lwz       r12, 0x1B8(r28);
  stb       r11, 0x887(r12);
  lha       r11, 0xB8(r12);
  stb       r11, 0x884(r12);

loc_0x5C:
  blr;
    )
}

// Crown for 1st place on minimap (Nametag removed due to crashes)
kmRuntimeUse(0x807EB490); // Crown display

void CrownSetting() {
    // Reset to default (disabled)
    kmRuntimeWrite32A(0x807EB490, 0x7C84002E);
    
    // Check if Crown is enabled in settings
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_CROWN) == CROWN_ENABLED) {
        // Enable Crown display
        kmRuntimeCallA(0x807EB490, Crown);
    }
}
static PageLoadHook CROWN(CrownSetting);

//Change map_chara to nap_chara [Toadette Hack Fan] (da IKW UI/IKWUI.cpp)
kmWrite8(0x808A94E6, 0x6E);

kmRuntimeUse(0x805A20E4);
kmRuntimeUse(0x80892324);

void FovSetting() {
    kmRuntimeWrite32A(0x805A20E4, 0x4082000C);
    kmRuntimeWrite32A(0x80892324, 0x3F000000);
    
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_FOV) == FOV_4_3) {
        kmRuntimeWrite32A(0x805A20E4, 0x60000000);
    }
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_FOV) == FOV_16_9) {
        kmRuntimeWrite32A(0x805A20E4, 0x4800000C);
    }
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_FOV) == FOV_CUSTOM) {
        kmRuntimeWrite32A(0x80892324, 0x3F400000);
    }
}
static PageLoadHook FOV(FovSetting);

}
}
