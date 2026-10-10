#ifndef _RAINBOW_HUD_
#define _RAINBOW_HUD_

#include <kamek.hpp>

namespace Pulsar {
namespace UI {

class RainbowHUD {
public:
    static void Init();
    static void Update();
    static void GetColors(u8& r, u8& g, u8& b);
    static void Reset();
    
private:
    static u8 red1, green1, blue1;
    static u8 red2, green2, blue2;
    static u8 red3, green3, blue3;
    static u8 red4, green4, blue4;
    static u8 red5, green5, blue5;
    static u8 red6, green6, blue6;
    
    static u8 rainbowStage;
    static u8 currentColorIndex;
};

}
}

#endif
