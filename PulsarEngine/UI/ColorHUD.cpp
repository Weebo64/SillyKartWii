#include <SillyKartWii.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace UI {

static u8 hudR = 255;
static u8 hudG = 255;
static u8 hudB = 255;

static const u8 hudColors[13][3] = {
    {255, 255, 255},
    {60, 60, 60},
    {208, 1, 1},
    {255, 141, 0},
    {233, 212, 0},
    {7, 255, 0},
    {8, 39, 245},
    {144, 0, 255},
    {255, 201, 237},
    {255, 0, 255},
    {36, 224, 255},
    {0, 128, 128},
    {212, 175, 55}
};

void UpdateHUDColor() {
    u8 setting = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_SCROLL_HUDCOLOR);
    
    if (setting == 13) {
        hudR = 176;
        hudG = 11;
        hudB = 105;
    } else {
        if (setting >= 13) setting = 0;
        hudR = hudColors[setting][0];
        hudG = hudColors[setting][1];
        hudB = hudColors[setting][2];
    }
}

void GetHUDColor(void* self, RGBA16* c0, RGBA16* c1) {
    UpdateHUDColor();
    c0->red = hudR;
    c0->green = hudG;
    c0->blue = hudB;
    c0->alpha = 0xFD;
    c1->red = hudR;
    c1->green = hudG;
    c1->blue = hudB;
    c1->alpha = 0xFD;
}
kmBranch(0x805f03dc, GetHUDColor);
kmBranch(0x805f0440, GetHUDColor);

void GetHUDBaseColor(void* self, RGBA16* c) {
    UpdateHUDColor();
    c->red = 0;
    c->green = 0;
    c->blue = 0;
    c->alpha = 0x46;
}
kmBranch(0x805f04d8, GetHUDBaseColor);

}
}
