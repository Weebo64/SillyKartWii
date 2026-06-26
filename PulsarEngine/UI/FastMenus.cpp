#include <kamek.hpp>
#include <MarioKartWii/UI/Page/Page.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <Settings/Settings.hpp>
#include <SillyKartWii.hpp>
#include <RuntimeWrite.hpp>

namespace Pulsar {
namespace UI {
//Adapted from east_'s code
static void FasterMenusOnSceneChange(SectionMgr* sectionMgr, u32 delay, u32 color) {
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FASTMENUS) == MENUSETTING_FASTMENUS_ENABLED) delay = 0;
    sectionMgr->RequestSceneChange(delay, color);
}
kmCall(0x80602510, FasterMenusOnSceneChange);

asmFunc FastMenusButSlower() {
    ASM(
  cmpwi     r4, 0x3;
  bne-      loc_0x10;
  lis       r12, 0x42FE;
  stw       r12, 0x8(r23);

loc_0x10:
  lfs       f1, 0x8(r23);
  blr;
    )
}
//Adapted from east_'s code
kmRuntimeUse(0x805C31A0);
static void FasterMenuPatchTransitionDelay() {
    const u8 val = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MENU, SETTINGMENU_RADIO_FASTMENUS);
    float transitionDelay = 176.0f;
    F32_MENUSPEED = 0.0f;
    if(val == MENUSETTING_FASTMENUS_ENABLED) {
        transitionDelay -= transitionDelay;
        F32_MENUSPEED = 300.0f;
    }
    kmRuntimeWrite32A(0x805C31A0, 0xC0370008);
    if(val == 1) {
        kmRuntimeCallA(0x805C31A0, FastMenusButSlower);
    }
    Page::transitionDelay = transitionDelay;
};
static Settings::Hook FasterMenus(FasterMenuPatchTransitionDelay);
BootHook FasterMenusBoot(FasterMenuPatchTransitionDelay, 3);

}//namespace UI
}//namespace Pulsar