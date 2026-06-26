#include <kamek.hpp>
#include <Settings/SettingsParam.hpp>
#include <PulsarSystem.hpp>
#include <SillyKartWii.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <Gamemodes/FreeRoam/FRMgr.hpp>

namespace Pulsar {
namespace UI {

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

}
}
