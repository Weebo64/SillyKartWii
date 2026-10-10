#ifndef _PULSAR_COUNTDOWN_
#define _PULSAR_COUNTDOWN_
#include <kamek.hpp>

namespace Pulsar {
namespace Countdown {

static const u32 endFrame = 0x2328;
static const u32 musicRampFrame = 0x189C;
static const u32 hitBonusFrames = 0xB4;
static const float displayStepPerPoint = 0.0f;
static const float displayCap = 0.0f;
static const u32 scoreAxeAt = 10;
static const u8 raceLapCount = 8;

bool IsEnabled();
u32 GetTimeLimitMs();
u8 GetLapCount(u8 kmpLapCount);

}  // namespace Countdown
}  // namespace Pulsar
#endif
