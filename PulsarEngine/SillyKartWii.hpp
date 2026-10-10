#ifndef _SILLYKART_
#define _SILLYKART_
#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <types.hpp>

extern u8 characterId;
extern u8 vehicleId;
extern u8 REGION;
extern u32 FPSPatchHook;
extern u32 PredictionHook;
extern u32 ItemRainOnlineFixHook;
extern u32 BloomHook;

extern u8 characterId;
extern u8 vehicleId;

extern u8 U8_RED1;
extern u8 U8_GREEN1;
extern u8 U8_BLUE1;
extern u8 U8_ALPHA1;
extern u8 U8_RED2;
extern u8 U8_GREEN2;
extern u8 U8_BLUE2;
extern u8 U8_ALPHA2;
extern u8 U8_RED3;
extern u8 U8_GREEN3;
extern u8 U8_BLUE3;
extern u8 U8_ALPHA3;
extern u8 U8_RED4;
extern u8 U8_GREEN4;
extern u8 U8_BLUE4;
extern u8 U8_ALPHA4;
extern u8 U8_RED5;
extern u8 U8_GREEN5;
extern u8 U8_BLUE5;
extern u8 U8_ALPHA5;
extern u8 U8_RED6;
extern u8 U8_GREEN6;
extern u8 U8_BLUE6;
extern u8 U8_ALPHA6;

extern u8 U8_SILLY_MODE;
extern u16 U16_SILLY_EFFECTS;
extern u16 U16_SILLY_SOUNDS;
extern u8 U8_SILLY_PHYSICS;

extern u8 FontRename;
extern u8 RaceRename;
extern u8 CommonRename;
extern u8 AwardRename;

extern u16 WiiInput;
extern u16 GCInput;
extern u16 ClassicInput;
extern u16 U16_FREE_ROAM;
extern u16 U16_FCOFFLINE;
extern u16 U16_FCONLINE;
extern u8 U8_BATTLE_CHECK;
extern u16 U16_CROWN;
extern u16 U16_DOUBLE_ITEMS;

// Character Layers - Model IDs
extern u16 U16_MARIO_MODEL;
extern u16 U16_BABY_PEACH_MODEL;
extern u16 U16_WALUIGI_MODEL;
extern u16 U16_BOWSER_MODEL;
extern u16 U16_BABY_DAISY_MODEL;
extern u16 U16_DRY_BONES_MODEL;
extern u16 U16_BABY_MARIO_MODEL;
extern u16 U16_LUIGI_MODEL;
extern u16 U16_TOAD_MODEL;
extern u16 U16_DONKEY_KONG_MODEL;
extern u16 U16_YOSHI_MODEL;
extern u16 U16_WARIO_MODEL;
extern u16 U16_BABY_LUIGI_MODEL;
extern u16 U16_TOADETTE_MODEL;
extern u16 U16_KOOPA_TROOPA_MODEL;
extern u16 U16_DAISY_MODEL;
extern u16 U16_PEACH_MODEL;
extern u16 U16_BIRDO_MODEL;
extern u16 U16_DIDDY_KONG_MODEL;
extern u16 U16_KING_BOO_MODEL;
extern u16 U16_BOWSER_JR_MODEL;
extern u16 U16_DRY_BOWSER_MODEL;
extern u16 U16_FUNKY_KONG_MODEL;
extern u16 U16_ROSALINA_MODEL;

// Character Layers - Icon IDs
extern u8 U8_MARIO_ICON;
extern u8 U8_BABY_PEACH_ICON;
extern u8 U8_WALUIGI_ICON;
extern u8 U8_BOWSER_ICON;
extern u8 U8_BABY_DAISY_ICON;
extern u8 U8_DRY_BONES_ICON;
extern u8 U8_BABY_MARIO_ICON;
extern u8 U8_LUIGI_ICON;
extern u8 U8_TOAD_ICON;
extern u8 U8_DONKEY_KONG_ICON;
extern u8 U8_YOSHI_ICON;
extern u8 U8_WARIO_ICON;
extern u8 U8_BABY_LUIGI_ICON;
extern u8 U8_TOADETTE_ICON;
extern u8 U8_KOOPA_TROOPA_ICON;
extern u8 U8_DAISY_ICON;
extern u8 U8_PEACH_ICON;
extern u8 U8_BIRDO_ICON;
extern u8 U8_DIDDY_KONG_ICON;
extern u8 U8_KING_BOO_ICON;
extern u8 U8_BOWSER_JR_ICON;
extern u8 U8_DRY_BOWSER_ICON;
extern u8 U8_FUNKY_KONG_ICON;
extern u8 U8_ROSALINA_ICON;


// Transmission & Brake Drifting
extern u8 U8_BRAKEDRIFTING;
extern u16 U16_GAMEPLAY2;
extern u32 TTS_CHECK;
extern u16 U16_MISSION_MODE_FIX;
extern u8 U8_MISSION_ID;
extern u8 U8_WWS_CHECK;
extern float F32_MENUSPEED;

// Countdown Mode variables (defined as extern "C" in Countdown/Countdown.cpp)
// extern u16 U16_GAMEPLAYG;
// extern u16 EndRaceCountdown;
// extern u8 LAPNUMBER;
// extern float U32_MUSIC_SPEED;

// ItemRain Mode
extern u16 U16_ITEMRAIN;

namespace SillyKartWii {

extern bool isPAL;
extern bool isUSA;
extern bool isJapan;
extern bool isKorea;

void HideChannelButton();

class System : public Pulsar::System {
public:
    static Pulsar::System* Create();
    static bool IsSillyModeEnabled();
    static bool IsCustomPhysicsEnabled();
    
    enum SillyMode {
        SILLY_MODE_NORMAL,
        SILLY_MODE_CHAOS,
        SILLY_MODE_EXTREME
    };
    
    enum WeightClass {
        LIGHTWEIGHT,
        MEDIUMWEIGHT,
        HEAVYWEIGHT,
        MIIS,
        ALLWEIGHT
    };
    
    SillyMode currentMode;
    WeightClass weight;
    
    static WeightClass GetWeightClass(CharacterId id);
};

}

#endif