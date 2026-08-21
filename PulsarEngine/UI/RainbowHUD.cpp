#include <UI/RainbowHUD.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace UI {

u8 RainbowHUD::red1 = 255;
u8 RainbowHUD::green1 = 0;
u8 RainbowHUD::blue1 = 0;
u8 RainbowHUD::red2 = 0;
u8 RainbowHUD::green2 = 255;
u8 RainbowHUD::blue2 = 0;
u8 RainbowHUD::red3 = 0;
u8 RainbowHUD::green3 = 0;
u8 RainbowHUD::blue3 = 255;
u8 RainbowHUD::red4 = 255;
u8 RainbowHUD::green4 = 255;
u8 RainbowHUD::blue4 = 0;
u8 RainbowHUD::red5 = 255;
u8 RainbowHUD::green5 = 0;
u8 RainbowHUD::blue5 = 255;
u8 RainbowHUD::red6 = 0;
u8 RainbowHUD::green6 = 255;
u8 RainbowHUD::blue6 = 255;

u8 RainbowHUD::rainbowStage = 0;
u8 RainbowHUD::currentColorIndex = 1;

static u8* const HUD_COLOR_R = reinterpret_cast<u8*>(0x80815088);
static u8* const HUD_COLOR_G = reinterpret_cast<u8*>(0x80815089);
static u8* const HUD_COLOR_B = reinterpret_cast<u8*>(0x8081508A);

static u8* const POS_COLOR_R = reinterpret_cast<u8*>(0x80815088);
static u8* const POS_COLOR_G = reinterpret_cast<u8*>(0x80815089);
static u8* const POS_COLOR_B = reinterpret_cast<u8*>(0x8081508A);

void RainbowHUD::Init() {
    red1 = 255; green1 = 0; blue1 = 0;
    red2 = 0; green2 = 255; blue2 = 0;
    red3 = 0; green3 = 0; blue3 = 255;
    red4 = 255; green4 = 255; blue4 = 0;
    red5 = 255; green5 = 0; blue5 = 255;
    red6 = 0; green6 = 255; blue6 = 255;
    rainbowStage = 0;
    currentColorIndex = 1;
}

void RainbowHUD::Update() {
    const float speed = 1.5f;
    
    for(float i = 0; i < speed; i += 1.0f) {
        switch(rainbowStage) {
            case 0:
                if(green1 < 0xFF) {
                    green1++;
                } else {
                    rainbowStage = 1;
                    currentColorIndex = 1;
                }
                break;
                
            case 1:
                if(red2 < 0xFF) {
                    red2++;
                } else {
                    rainbowStage = 2;
                }
                break;
                
            case 2:
                if(red1 > 0) {
                    red1--;
                } else {
                    rainbowStage = 3;
                    currentColorIndex = 2;
                }
                break;
                
            case 3:
                if(blue3 < 0xFF) {
                    blue3++;
                } else {
                    rainbowStage = 4;
                }
                break;
                
            case 4:
                if(blue1 < 0xFF) {
                    blue1++;
                } else {
                    rainbowStage = 5;
                    currentColorIndex = 3;
                }
                break;
                
            case 5:
                if(green4 > 0) {
                    green4--;
                } else {
                    rainbowStage = 6;
                }
                break;
                
            case 6:
                if(green1 > 0) {
                    green1--;
                } else {
                    rainbowStage = 7;
                    currentColorIndex = 4;
                }
                break;
                
            case 7:
                if(red5 < 0xFF) {
                    red5++;
                } else {
                    rainbowStage = 8;
                }
                break;
                
            case 8:
                if(red1 < 0xFF) {
                    red1++;
                } else {
                    rainbowStage = 9;
                    currentColorIndex = 5;
                }
                break;
                
            case 9:
                if(green6 < 0xFF) {
                    green6++;
                } else {
                    rainbowStage = 10;
                }
                break;
                
            case 10:
                if(blue1 > 0) {
                    blue1--;
                } else {
                    rainbowStage = 0;
                    currentColorIndex = 1;
                }
                break;
        }
    }

    *HUD_COLOR_R = red1;
    *HUD_COLOR_G = green1;
    *HUD_COLOR_B = blue1;
}

void RainbowHUD::GetColors(u8& r, u8& g, u8& b) {
    r = red1;
    g = green1;
    b = blue1;
}

void RainbowHUD::Reset() {
    Init();
}

static void UpdateRainbowEveryFrame() {
    u8 rainbowSetting = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_MISC, SETTINGMISC_RADIO_RAINBOW_HUD);
    
    if(rainbowSetting == MISCSETTING_RAINBOW_HUD_ENABLED) {
        RainbowHUD::Update();
    }
}

static RaceFrameHook rainbowFrameHook(UpdateRainbowEveryFrame);

}
}
