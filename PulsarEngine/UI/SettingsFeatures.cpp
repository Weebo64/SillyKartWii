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
    if(Racedata::sInstance->menusScenario.settings.gamemode != MODE_TIME_TRIAL) FreeRoam::U16_FREE_ROAM = 0x0000;

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

}
}
