#include <kamek.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Race/RaceData.hpp>

namespace Pulsar {
namespace Race {

kmWrite32(0x80554224, 0x3C808000);
kmWrite32(0x80554228, 0x88841200);
kmWrite32(0x8055422C, 0x48000044);

kmRuntimeUse(0x80001200);

void ApplyFPSSetting() {
    const Racedata* racedata = Racedata::sInstance;
    if(racedata == nullptr) return;
    
    const Settings::Mgr& settings = Settings::Mgr::Get();
    u8 fpsSetting = settings.GetSettingValue(Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FPS);
    
    *reinterpret_cast<u8*>(kmRuntimeAddr(0x80001200)) = (fpsSetting == MENUSETTING_FPS_30) ? 1 : 0;
}

void EnforceFPSSetting() {
    const Racedata* racedata = Racedata::sInstance;
    if(racedata == nullptr) return;
    
    const Settings::Mgr& settings = Settings::Mgr::Get();
    u8 fpsSetting = settings.GetSettingValue(Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FPS);
    *reinterpret_cast<u8*>(kmRuntimeAddr(0x80001200)) = (fpsSetting == MENUSETTING_FPS_30) ? 1 : 0;
}

static SectionLoadHook ApplyFPS(ApplyFPSSetting);
static RaceLoadHook ApplyFPSRace(ApplyFPSSetting);
static RaceFrameHook EnforceFPS(EnforceFPSSetting);

} //namespace Race
} //namespace Pulsar
