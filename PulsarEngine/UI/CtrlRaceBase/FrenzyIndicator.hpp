#ifndef _FRENZYINDICATOR_
#define _FRENZYINDICATOR_

#include <kamek.hpp>
#include <MarioKartWii/UI/Ctrl/CtrlRace/CtrlRaceBase.hpp>
#include <UI/CtrlRaceBase/CustomCtrlRaceBase.hpp>

namespace Pulsar {
namespace UI {

class CtrlRaceFrenzy : public CtrlRaceBase {
public:
    void OnUpdate() override;
    static u32 Count();
    static void Create(Page& page, u32 index, u32 count);

private:
    void Load();
    void ApplyPlacement();
    bool lastActive;
    u32 animationFrame;
};

} // namespace UI
} // namespace Pulsar

#endif