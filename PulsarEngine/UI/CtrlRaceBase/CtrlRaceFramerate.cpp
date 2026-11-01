#include <UI/CtrlRaceBase/CtrlRaceFramerate.hpp>
#include <UI/CtrlRaceBase/CustomCtrlRaceBase.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>

extern float CurrentFramerate; // provided by FramerateMonitor.cpp

namespace Pulsar {
namespace UI {

u32 CtrlRaceFramerate::Count() {
    if(Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE2, SETTINGRACE2_RADIO_FPS_DISPLAY) == RACE2SETTING_FPS_DISPLAY_DISABLED) return 0;
    return 1; // single HUD element
}

void CtrlRaceFramerate::Create(Page& page, u32 index, u32 count) {
    for(int i = 0; i < (int)count; ++i) {
        CtrlRaceFramerate* fm = new(CtrlRaceFramerate);
        page.AddControl(index + i, *fm, 0);
        char variant[0x20];
        int pos = i;
        if(count == 1 && Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_SOM) == RACESETTING_SOM_RIGHT) pos = 1;
        snprintf(variant, 0x20, "Framerate_%1d_%1d", 1, pos);
        fm->Load(variant, i);
    }
}

static CustomCtrlBuilder FRAMEMONITOR(CtrlRaceFramerate::Count, CtrlRaceFramerate::Create);

void CtrlRaceFramerate::Load(const char* variant, u8 id) {
    this->hudSlotId = id;
    ControlLoader loader(this);
    const char* anims[] = {
        "Hundreds", "Hundreds", nullptr,
        "Tens", "Tens", nullptr,
        "Units", "Units", nullptr,
        "Dot", "Dot",nullptr,
        "Tenths", "Tenths", nullptr,
        "Hundredths", "Hundredths", nullptr,
        "Thousandths", "Thousandths", nullptr,
        nullptr
    };

    loader.Load(UI::raceFolder, "PULFramerate", variant, anims);

    this->Animate();
    return;
}

void CtrlRaceFramerate::Init() {
    this->HudSlotColorEnable("speed0", true);
    this->HudSlotColorEnable("speed1", true);
    this->HudSlotColorEnable("speed2", true);
    this->HudSlotColorEnable("speed3", true);
    this->HudSlotColorEnable("speed4", true);
    this->HudSlotColorEnable("speed5", true);
    this->HudSlotColorEnable("speed6", true);
    this->HudSlotColorEnable("kmh", true);
    this->updateCounter = 0;
    this->lastFrValue = 0;
    LayoutUIControl::Init();
    return;
}

void CtrlRaceFramerate::OnUpdate() {
    this->UpdatePausePosition();

    // update the displayed value only every 10 cycles to reduce jitter
    this->updateCounter++;
    if(this->updateCounter >= 10) {
        this->updateCounter = 0;
        float fps = CurrentFramerate;
        this->lastFrValue = static_cast<u32>(fps * 1000.0f); // keep three decimal digits internally, display only two
    }
    u32 frValue = this->lastFrValue;

    u32 hundreds = frValue % 1000000 / 100000;
    u32 tens = frValue % 100000 / 10000;
    u32 units = frValue % 10000 / 1000;
    u32 dot = 11; // always show dot
    u32 tenths = frValue % 1000 / 100;
    u32 hundredths = frValue % 100 / 10;
    u32 thousandths = 10;

    if(frValue < 10000) { // shift everything by 2 to the left
        hundreds = units;
        tens = dot;
        units = tenths;
        dot = hundredths;
        tenths = thousandths;
        hundredths = 10;
    }
    else if(frValue < 100000) {
        hundreds = tens;
        tens = units;
        units = dot;
        dot = tenths;
        tenths = hundredths;
        hundredths = thousandths;
    }

    SpeedArg args(hundreds, tens, units, dot, tenths, hundredths, thousandths);
    this->Animate(&args);
    return;
}

void CtrlRaceFramerate::Animate(const SpeedArg* args) {
    for(int i = 0; i < 7; ++i) {
        AnimationGroup& group = this->animator.GetAnimationGroupById(i);
        float frame = 0.0f;
        if(args != nullptr) frame = static_cast<float>(args->values[i]);
        group.PlayAnimationAtFrameAndDisable(0, frame);
    }
}

}//namespace UI
}//namespace Pulsar