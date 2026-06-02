#ifndef _SILLYKART_
#define _SILLYKART_
#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <types.hpp>
#include <CustomCharacters/CustomCharacters.hpp>

extern u8 REGION;
extern u32 FPSPatchHook;
extern u32 PredictionHook;
extern u32 ItemRainOnlineFixHook;
extern u32 BloomHook;

extern u8 U8_RED1;
extern u8 U8_GREEN1;
extern u8 U8_BLUE1;
extern u8 U8_ALPHA1;

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

// Transmission & Brake Drifting
extern u8 U8_BRAKEDRIFTING;
extern u16 U16_GAMEPLAY2;
extern u32 TTS_CHECK;
extern u16 U16_MISSION_MODE_FIX;
extern u8 U8_WWS_CHECK;
extern float F32_MENUSPEED;

namespace SillyKartWii {

extern bool isPAL;
extern bool isUSA;
extern bool isJapan;
extern bool isKorea;

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