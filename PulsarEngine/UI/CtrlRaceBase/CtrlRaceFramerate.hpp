#ifndef _PULFRAMEM_
#define _PULFRAMEM_
#include <kamek.hpp>
#include <UI/CtrlRaceBase/CustomCtrlRaceBase.hpp>

namespace Pulsar {
namespace UI {
class CtrlRaceFramerate : public CtrlRaceBase {
public:
    void Init() override;
    void OnUpdate() override;
    static u32 Count();
    static void Create(Page& page, u32 index, u32 count);
private:
    struct SpeedArg {
        SpeedArg() {}
        SpeedArg(u32 hundreds, u32 tens, u32 units, u32 dot, u32 tenths, u32 hundredths, u32 thousandths) :
            hundreds(hundreds), tens(tens), units(units), dot(dot), tenths(tenths), hundredths(hundredths), thousandths(thousandths) {}
        union {
            u32 values[7];
            struct {
                u32 hundreds;
                u32 tens;
                u32 units;
                u32 dot;
                u32 tenths;
                u32 hundredths;
                u32 thousandths;
            };
        };
    };
    void Load(const char* variant, u8 id);
    void Animate(const SpeedArg* args = nullptr);
    u8 updateCounter;
    u32 lastFrValue;
};
}
}

#endif