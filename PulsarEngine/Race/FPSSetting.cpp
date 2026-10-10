#include <kamek.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Scene/GameScene.hpp>
#include <PulsarSystem.hpp>
#include <SillyKartWii.hpp>

namespace Pulsar {
namespace Race {

void FPSPatch() {
    GameScene *scene = const_cast<GameScene *>(GameScene::GetCurrent());
    bool use30FPS = Pulsar::Settings::Mgr::Get().GetSettingValue(Pulsar::Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FPS) == MENUSETTING_FPS_30;
    
    // Force 30fps for ItemRain mode
    if(System::sInstance && System::sInstance->IsContext(PULSAR_ITEMMODERAIN)) {
        use30FPS = true;
    }
    
    scene->SetFramerate(use30FPS ? 1 : 0);
}

static SectionLoadHook PatchFPS(FPSPatch);
static RaceLoadHook PatchFPSOnRaceLoad(FPSPatch);

}
}