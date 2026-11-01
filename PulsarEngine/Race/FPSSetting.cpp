#include <kamek.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Scene/GameScene.hpp>
#include <core/egg/Scene.hpp>
#include <PulsarSystem.hpp>
#include <SillyKartWii.hpp>
#include <Race/Players24/Players24.hpp>
#include <MarioKartWii/Race/RaceData.hpp>

namespace Pulsar {
namespace Race {

void FPSPatch() {
    GameScene *scene = const_cast<GameScene *>(GameScene::GetCurrent());
    bool use30FPS = Pulsar::Settings::Mgr::Get().GetSettingValue(Pulsar::Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FPS) == MENUSETTING_FPS_30;
    
    // Force 30fps for ItemRain mode
    if(System::sInstance && System::sInstance->IsContext(PULSAR_ITEMMODERAIN)) {
        use30FPS = true;
    }

    // bumboblatclat 30fps for when da mor dan 13 players in ze raceseses
    if (scene->id == SCENE_ID_RACE) {
        u8 playerCount = Players24::GetRacePlayerCount();
        if (playerCount >= 13) {
            use30FPS = true;
        }
    }
    
    scene->SetFramerate(use30FPS ? 1 : 0);
}

static SectionLoadHook PatchFPS(FPSPatch);
static RaceLoadHook PatchFPSOnRaceLoad(FPSPatch);

}
}
