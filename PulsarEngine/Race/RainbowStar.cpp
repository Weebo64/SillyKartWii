#include <kamek.hpp>
#include <MarioKartWii/Kart/KartManager.hpp>

namespace Pulsar {
namespace Race {

// 1, 2, 4, 7 (empfohlen), 15
static const u8 SPEED = 7;

static u8 sTimer;
static u8 sPhase;

static const u8 SWIZZLE[6][3] = {
    {0x14, 0x16, 0x16},
    {0x14, 0x14, 0x16},
    {0x16, 0x14, 0x16},
    {0x16, 0x14, 0x14},
    {0x16, 0x16, 0x14},
    {0x14, 0x16, 0x14}
};

static void RainbowStarFrame() {
    sTimer += SPEED;
    if (sTimer >= 0x30) {
        sTimer = 0;
        ++sPhase;
        if (sPhase >= 6) sPhase = 0;
    }

    // PAL SetModelColorsImpl: lbz-Offsets R/B swizzlen
    *reinterpret_cast<u8*>(0x8056BEB3) = SWIZZLE[sPhase][0];
    *reinterpret_cast<u8*>(0x8056BEBB) = SWIZZLE[sPhase][1];
    *reinterpret_cast<u8*>(0x8056BEC3) = SWIZZLE[sPhase][2];
}
static RaceFrameHook rainbowStarHook(RainbowStarFrame);

} // namespace Race
} // namespace Pulsar