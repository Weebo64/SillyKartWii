#include <SillyKartWii.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <SlotExpansion/CupsConfig.hpp>
#include <Settings/UI/SettingsPanel.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/Kart/KartValues.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <core/rvl/OS/OS.hpp>

namespace SillyKartWii {
bool isPAL = REGION == 'P';
bool isUSA = REGION == 'E';
bool isJapan = REGION == 'J';
bool isKorea = REGION == 'K';
}

u32 FPSPatchHook = 0;
u32 PredictionHook = 0;
u32 ItemRainOnlineFixHook = 0;

// HUD Color variables (initialized to white with full alpha)
u8 U8_RED1 = 0xFF;
u8 U8_GREEN1 = 0xFF;
u8 U8_BLUE1 = 0xFF;
u8 U8_ALPHA1 = 0xFF;
u8 U8_RED2 = 0xFF;
u8 U8_GREEN2 = 0xFF;
u8 U8_BLUE2 = 0xFF;
u8 U8_ALPHA2 = 0xFF;
u8 U8_RED3 = 0xFF;
u8 U8_GREEN3 = 0xFF;
u8 U8_BLUE3 = 0xFF;
u8 U8_ALPHA3 = 0xFF;
u8 U8_RED4 = 0xFF;
u8 U8_GREEN4 = 0xFF;
u8 U8_BLUE4 = 0xFF;
u8 U8_ALPHA4 = 0xFF;
u8 U8_RED5 = 0xFF;
u8 U8_GREEN5 = 0xFF;
u8 U8_BLUE5 = 0xFF;
u8 U8_ALPHA5 = 0xFF;
u8 U8_RED6 = 0xFF;
u8 U8_GREEN6 = 0xFF;
u8 U8_BLUE6 = 0xFF;
u8 U8_ALPHA6 = 0xFF;

u8 U8_SILLY_MODE = 0;
u16 U16_SILLY_EFFECTS = 0;
u16 U16_SILLY_SOUNDS = 0;
u8 U8_SILLY_PHYSICS = 0;

// Transmission & Brake Drifting variables
u8 U8_BRAKEDRIFTING = 0;
u16 U16_GAMEPLAY2 = 0;
u32 TTS_CHECK = 0;
u16 U16_MISSION_MODE_FIX = 0;
u8 U8_WWS_CHECK = 0;
float F32_MENUSPEED = 0.0f;

u16 WiiInput = 0;
u16 GCInput = 0;
u16 ClassicInput = 0;
u16 U16_FREE_ROAM = 0;
u8 U8_BATTLE_CHECK = 0;

namespace SillyKartWii {

Pulsar::System* System::Create() {
    return new System();
}
Pulsar::System::Inherit CreateSillyKartWii(System::Create);

bool System::IsSillyModeEnabled() {
    return U8_SILLY_MODE != 0;
}

bool System::IsCustomPhysicsEnabled() {
    return U8_SILLY_PHYSICS != 0;
}

void FPSPatch() {
    FPSPatchHook = 0x00;
    const GameMode mode = Racedata::sInstance->racesScenario.settings.gamemode;
    
    if (System::IsSillyModeEnabled()) {
        FPSPatchHook = 0x00FF0100;
    }
}
static SectionLoadHook PatchFPS(FPSPatch);

static void InitializeSillyKartWii() {
    U8_SILLY_MODE = 0;
    U8_SILLY_PHYSICS = 0;
    U16_SILLY_EFFECTS = 0;
    U16_SILLY_SOUNDS = 0;
}
BootHook initSillyKart(InitializeSillyKartWii, 0);

}
