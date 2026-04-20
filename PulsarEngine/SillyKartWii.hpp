#ifndef _SILLYKART_
#define _SILLYKART_
#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Settings/Settings.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <types.hpp>

extern u8 REGION;
extern u32 FPSPatchHook;
extern u32 PredictionHook;
extern u32 ItemRainOnlineFixHook;
extern u32 BloomHook;

// HUD Color variables (for custom color support)
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

extern u16 CUSTOM_MARIO;
extern u16 CUSTOM_BABY_PEACH;
extern u16 CUSTOM_WALUIGI;
extern u16 CUSTOM_BOWSER;
extern u16 CUSTOM_BABY_DAISY;
extern u16 CUSTOM_DRY_BONES;
extern u16 CUSTOM_BABY_MARIO;
extern u16 CUSTOM_LUIGI;
extern u16 CUSTOM_TOAD;
extern u16 CUSTOM_DONKEY_KONG;
extern u16 CUSTOM_YOSHI;
extern u16 CUSTOM_WARIO;
extern u16 CUSTOM_BABY_LUIGI;
extern u16 CUSTOM_TOADETTE;
extern u16 CUSTOM_KOOPA_TROOPA;
extern u16 CUSTOM_DAISY;
extern u16 CUSTOM_PEACH;
extern u16 CUSTOM_BIRDO;
extern u16 CUSTOM_DIDDY_KONG;
extern u16 CUSTOM_KING_BOO;
extern u16 CUSTOM_BOWSER_JR;
extern u16 CUSTOM_DRY_BOWSER;
extern u16 CUSTOM_FUNKY_KONG;
extern u16 CUSTOM_ROSALINA;
extern u16 CUSTOM_PEACH_MENU;
extern u16 CUSTOM_DAISY_MENU;
extern u16 CUSTOM_ROSALINA_MENU;
extern u8 CUSTOM_DRIVER;

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