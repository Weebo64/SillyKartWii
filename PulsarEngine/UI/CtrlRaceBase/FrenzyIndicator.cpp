#include <UI/CtrlRaceBase/FrenzyIndicator.hpp>
#include <UI/UI.hpp>
#include <Extensions/ItemExpansion/FrenzyMode.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <MarioKartWii/UI/Text/Text.hpp>
#include <core/rvl/os/OS.hpp>
#include <core/nw4r/ut/Color.hpp>
#include <MarioKartWii/3D/GameScreenEffects/FilterEffect.hpp>

namespace Pulsar {
namespace UI {

static const float FRENZY_POS_X = 25.0f;
static const float FRENZY_POS_Y = 260.0f;
static const float FRENZY_SCALE = 0.70f;

u32 CtrlRaceFrenzy::Count() {
    if (!Racedata::sInstance) return 1;
    u32 localCount = Racedata::sInstance->racesScenario.localPlayerCount;
    if (localCount == 0) localCount = 1;
    return localCount;
}

void CtrlRaceFrenzy::Create(Page& page, u32 index, u32 count) {
    for (u32 i = 0; i < count; ++i) {
        CtrlRaceFrenzy* ctrl = new (CtrlRaceFrenzy);
        page.AddControl(index + i, *ctrl, 0);
        ctrl->hudSlotId = static_cast<u8>(i);
        ctrl->lastActive = false;
        ctrl->animationFrame = 0;
        ctrl->Load();
    }
}

static CustomCtrlBuilder FRENZY_HUD(CtrlRaceFrenzy::Count, CtrlRaceFrenzy::Create);

void CtrlRaceFrenzy::ApplyPlacement() {
    lyt::Pane* text = this->layout.GetPaneByName("TextBox_00");
    if (text == nullptr) return;

    float animScale = FRENZY_SCALE;
    float animAlpha = 1.0f;
    float yOffset = 0.0f;
    
    if (animationFrame < 30) {
        float t = animationFrame / 30.0f;
        animScale = FRENZY_SCALE * (0.5f + t * 0.5f);
        animAlpha = t;
    }
    
    animScale += sin(animationFrame * 0.2f) * 0.1f;
    yOffset = sin(animationFrame * 0.15f) * 8.0f;
    
    float hue = (animationFrame * 6.0f);
    while (hue >= 360.0f) hue -= 360.0f;
    
    float r, g, b;
    if (hue < 60.0f) {
        r = 1.0f;
        g = hue / 60.0f;
        b = 0.0f;
    } else if (hue < 120.0f) {
        r = (120.0f - hue) / 60.0f;
        g = 1.0f;
        b = 0.0f;
    } else if (hue < 180.0f) {
        r = 0.0f;
        g = 1.0f;
        b = (hue - 120.0f) / 60.0f;
    } else if (hue < 240.0f) {
        r = 0.0f;
        g = (240.0f - hue) / 60.0f;
        b = 1.0f;
    } else if (hue < 300.0f) {
        r = (hue - 240.0f) / 60.0f;
        g = 0.0f;
        b = 1.0f;
    } else {
        r = 1.0f;
        g = 0.0f;
        b = (360.0f - hue) / 60.0f;
    }

    text->trans.x = FRENZY_POS_X;
    text->trans.y = FRENZY_POS_Y + yOffset;
    text->scale.x = animScale;
    text->scale.z = animScale;
    text->alpha = (u8)(animAlpha * 255);
    
    ut::Color rainbowColor;
    rainbowColor.r = (u8)(r * 255);
    rainbowColor.g = (u8)(g * 255);
    rainbowColor.b = (u8)(b * 255);
    rainbowColor.a = 255;
    
    text->SetVtxColor(0, rainbowColor);
    text->SetVtxColor(1, rainbowColor);
    text->SetVtxColor(2, rainbowColor);
    text->SetVtxColor(3, rainbowColor);
}

void CtrlRaceFrenzy::Load() {
    ControlLoader loader(this);
    loader.Load("game_image", "CTInfo", "CTInfo", nullptr);
    this->isHidden = true;
    this->animationFrame = 0;
    this->ApplyPlacement();
}

void CtrlRaceFrenzy::OnUpdate() {
    this->UpdatePausePosition();

    // Safety check: ensure FrenzyManager exists
    if (!Race::FrenzyManager::sInstance) {
        this->isHidden = true;
        return;
    }

    bool active = false;
    if (Race::FrenzyManager::sInstance) {
        active = Race::FrenzyManager::sInstance->IsFrenzyActive(this->GetPlayerId());
    }

    if (active && !this->lastActive) {
        Text::Info info;
        info.strings[0] = L"FRENZY!";
        this->SetMessage(BMG_TEXT, &info);
        this->animationFrame = 0;
    }
    
    if (active) {
        this->animationFrame++;
        this->ApplyPlacement();
        this->isHidden = false;
        
        float hue = (this->animationFrame * 4.0f);
        while (hue >= 360.0f) hue -= 360.0f;
        
        float r, g, b;
        if (hue < 60.0f) {
            r = 1.0f;
            g = hue / 60.0f;
            b = 0.0f;
        } else if (hue < 120.0f) {
            r = (120.0f - hue) / 60.0f;
            g = 1.0f;
            b = 0.0f;
        } else if (hue < 180.0f) {
            r = 0.0f;
            g = 1.0f;
            b = (hue - 120.0f) / 60.0f;
        } else if (hue < 240.0f) {
            r = 0.0f;
            g = (240.0f - hue) / 60.0f;
            b = 1.0f;
        } else if (hue < 300.0f) {
            r = (hue - 240.0f) / 60.0f;
            g = 0.0f;
            b = 1.0f;
        } else {
            r = 1.0f;
            g = 0.0f;
            b = (360.0f - hue) / 60.0f;
        }
        
        float intensity = 0.6f + sin(this->animationFrame * 0.12f) * 0.4f;
        ut::Color speedLinesColor;
        speedLinesColor.r = (u8)(r * 255);
        speedLinesColor.g = (u8)(g * 255);
        speedLinesColor.b = (u8)(b * 255);
        speedLinesColor.a = (u8)(intensity * 255);
        FilterEffectMgr::SetColor(speedLinesColor);
    } else {
        this->isHidden = true;
        this->animationFrame = 0;
        
        if (this->lastActive) {
            ut::Color clear;
            clear.r = 255;
            clear.g = 255;
            clear.b = 255;
            clear.a = 0;
            FilterEffectMgr::SetColor(clear);
        }
    }

    this->lastActive = active;
}

} // namespace UI
} // namespace Pulsar