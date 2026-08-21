#include <SillyKartWii.hpp>
#include <Settings/Settings.hpp>
#include <UI/RainbowHUD.hpp>

namespace Pulsar {
namespace UI {

static u8 hudR = 255;
static u8 hudG = 255;
static u8 hudB = 255;

static const u8 hudColors[14][3] = {
    {255, 255, 255},
    {60, 60, 60},
    {198, 0, 0},
    {240, 136, 10},
    {245, 200, 20},
    {2, 95, 2},
    {76, 255, 0},
    {8, 39, 205},
    {98, 20, 206},
    {235, 105, 210},
    {161, 185, 197},
    {36, 167, 240},
    {0, 160, 145},
    {207, 160, 45},
};

void UpdateHUDColor() {
    u8 setting = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_SCROLL_HUDCOLOR);
    
    if (setting == 14) {
        hudR = U8_RED1;
        hudG = U8_GREEN1;
        hudB = U8_BLUE1;
    } else {
        if (setting >= 14) setting = 0;
        hudR = hudColors[setting][0];
        hudG = hudColors[setting][1];
        hudB = hudColors[setting][2];
    }
}

void GetHUDColor(void* self, RGBA16* c0, RGBA16* c1) {
    u8 rainbowSetting = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_RADIO_RAINBOW_HUD);
    
    if(rainbowSetting == MISCSETTING_RAINBOW_HUD_ENABLED) {
        RainbowHUD::Update();
        u8 r, g, b;
        RainbowHUD::GetColors(r, g, b);
        hudR = r;
        hudG = g;
        hudB = b;
    } else {
        UpdateHUDColor();
    }
    
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
