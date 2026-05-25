#include <SillyKartWii.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace UI {

static u8 hudR = 255;
static u8 hudG = 255;
static u8 hudB = 255;
static bool colorInitialized = false;

static const u8 hudColors[13][3] = {
    {255, 255, 255}, // White
    {60, 60, 60},    // Dark Gray
    {255, 0, 0},     // Red
    {255, 141, 0},   // Orange
    {233, 212, 0},   // Yellow
    {4, 108, 5},     // Green
    {8, 39, 245},    // Blue
    {77, 15, 163},   // Purple
    {255, 109, 235}, // Pink
    {255, 0, 255},   // Magenta
    {36, 224, 255},  // Cyan
    {0, 128, 128},   // Teal
    {212, 175, 55}   // Gold
};

void UpdateHUDColor() {
    static u8 lastSetting = 255;
    u8 setting = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_SCROLL_HUDCOLOR);
    
    if (!colorInitialized || setting != lastSetting) {
        // #b00b69 hehe xDDDDDDDDDDDDDDDDDD
        if (setting == 13) {
            hudR = 176;
            hudG = 11;
            hudB = 105;
        } else {
            if (setting > 13) setting = 0;
            hudR = hudColors[setting][0];
            hudG = hudColors[setting][1];
            hudB = hudColors[setting][2];
        }
        lastSetting = setting;
        colorInitialized = true;
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