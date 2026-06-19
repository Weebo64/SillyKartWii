#include <kamek.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/Item/Obj/ObjProperties.hpp>
#include <MarioKartWii/Item/Obj/Gesso.hpp>
#include <MarioKartWii/Driver/DriverManager.hpp>
#include <MarioKartWii/Input/InputManager.hpp>
#include <MarioKartWii/CourseMgr.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <MarioKartWii/Race/RaceInfo/RaceInfo.hpp>
#include <MarioKartWii/Effect/EffectMgr.hpp>
#include <core/rvl/PAD.hpp>
#include <core/rvl/WPAD.hpp>
#include <PulsarSystem.hpp>

namespace Pulsar {
namespace Race {

static const u8 CHARGE_JUMP_MIN_TIME = 5;
static const u8 CHARGE_JUMP_MAX_TIME = 60;
static const float CHARGE_JUMP_MIN_SPEED = 15.0f;
static const float CHARGE_JUMP_SINK_SCALE = 0.85f;

struct ChargeJumpState {
    u8 chargeTimer[12];
    bool isCharging[12];
    bool wasHoldingButton[12];
    u8 rfhFrameCounter[12];
    Vec3 originalScale[12];
    bool hasOriginalScale[12];
};

static ChargeJumpState chargeJumpState = {};

bool g_chargeJumpUseFeather = false;

void ApplyChargeJumpFeatherEffect(Item::Player& itemPlayer) {
    const Kart::Pointers* pointers = itemPlayer.pointers;
    if (pointers == nullptr) return;
    
    Kart::Status* status = pointers->kartStatus;
    if (status == nullptr) return;
    
    Kart::Movement* kartMovement = pointers->kartMovement;
    if (kartMovement == nullptr) return;
    
    kartMovement->specialFloor |= 0x4;

    u32 type = 0x4;
    if ((status->bitfield1 & 0x4000) != 0) type = 0x2;
    status->jumpPadType = type;
    status->trickableTimer = 0x4;
}

void UseChargeJumpFeather(Item::Player& itemPlayer) {
    g_chargeJumpUseFeather = true;
    ApplyChargeJumpFeatherEffect(itemPlayer);
    g_chargeJumpUseFeather = false;
}

bool IsChargeJumpFeatherActive(u8 playerId) {
    return g_chargeJumpUseFeather;
}

void UpdateChargeJump() {
    if (!System::sInstance->IsContext(PULSAR_CHARGEJUMP)) {
        return;
    }

    Raceinfo* raceInfo = Raceinfo::sInstance;
    if (raceInfo == nullptr || raceInfo->stage >= RACESTAGE_IS_FINISHING) {
        return;
    }

    SectionMgr* sectionMgr = SectionMgr::sInstance;
    if (sectionMgr == nullptr) {
        return;
    }

    Item::Manager* itemManager = Item::Manager::sInstance;
    if (itemManager == nullptr) {
        return;
    }

    Racedata* racedata = Racedata::sInstance;
    if (racedata == nullptr) {
        return;
    }

    const SectionPad& pad = sectionMgr->pad;
    
    for (u8 hudSlotId = 0; hudSlotId < 4; ++hudSlotId) {
        Input::ControllerHolder* controllerHolder = pad.GetControllerHolder(hudSlotId);
        if (controllerHolder == nullptr || controllerHolder->inputStates == nullptr) {
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.wasHoldingButton[hudSlotId] = false;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            continue;
        }

        // Get actual playerId from hudSlotId
        const u32 playerId = racedata->GetPlayerIdOfLocalPlayer(hudSlotId);
        if (playerId >= itemManager->playerCount) {
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.wasHoldingButton[hudSlotId] = false;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            continue;
        }

        Item::Player& itemPlayer = itemManager->players[playerId];
        const Kart::Pointers* pointers = itemPlayer.pointers;
        if (pointers == nullptr) {
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.wasHoldingButton[hudSlotId] = false;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            continue;
        }

        Kart::Movement* kartMovement = pointers->kartMovement;
        if (kartMovement == nullptr) {
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.wasHoldingButton[hudSlotId] = false;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            continue;
        }

        Kart::Status* status = pointers->kartStatus;
        if (status == nullptr) {
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.wasHoldingButton[hudSlotId] = false;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            continue;
        }

        float currentSpeed = kartMovement->engineSpeed;

        u16 buttonRaw = controllerHolder->inputStates[0].buttonRaw;
        
        bool isBButtonHeld = false;
        bool wasButtonHeld = chargeJumpState.wasHoldingButton[hudSlotId];
        
        if (controllerHolder->inputStates) {
            const ControllerType controllerType = controllerHolder->curController->GetType();
            
            if (controllerType == GCN) {
                bool isBButtonPressed = (buttonRaw & PAD::PAD_BUTTON_B) != 0;
                bool isRButtonPressed = (buttonRaw & PAD::PAD_BUTTON_R) != 0;
                isBButtonHeld = isBButtonPressed && !isRButtonPressed;
            } else if (controllerType == NUNCHUCK) {
                bool isBButtonPressed = (buttonRaw & WPAD::WPAD_BUTTON_B) != 0;
                bool isZButtonPressed = (buttonRaw & WPAD::WPAD_BUTTON_Z) != 0;
                isBButtonHeld = isBButtonPressed && !isZButtonPressed;
            } else if (controllerType == WHEEL) {
                bool is2ButtonPressed = (buttonRaw & WPAD::WPAD_BUTTON_2) != 0;
                bool isBButtonPressed = (buttonRaw & WPAD::WPAD_BUTTON_B) != 0;
                isBButtonHeld = is2ButtonPressed && !isBButtonPressed;
            } else if (controllerType == CLASSIC) {
                bool isBButtonPressed = (buttonRaw & WPAD::WPAD_CL_BUTTON_B) != 0;
                bool isRButtonPressed = (buttonRaw & WPAD::WPAD_CL_TRIGGER_R) != 0;
                isBButtonHeld = isBButtonPressed && !isRButtonPressed;
            }
        }
        
        bool bButtonPressed = isBButtonHeld && !wasButtonHeld;

        if (bButtonPressed && !chargeJumpState.isCharging[hudSlotId] && currentSpeed >= CHARGE_JUMP_MIN_SPEED) {
            chargeJumpState.isCharging[hudSlotId] = true;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            
            chargeJumpState.originalScale[hudSlotId] = kartMovement->scale;
            chargeJumpState.hasOriginalScale[hudSlotId] = true;
        }

        if (isBButtonHeld && chargeJumpState.isCharging[hudSlotId]) {
            chargeJumpState.rfhFrameCounter[hudSlotId]++;
            
            if (chargeJumpState.chargeTimer[hudSlotId] < CHARGE_JUMP_MAX_TIME) {
                chargeJumpState.chargeTimer[hudSlotId]++;
            }
            
            if (status->airtime == 0) {
                kartMovement->hopFrame = 0;
                kartMovement->hopVelY = 0.0f;
                kartMovement->hopPosY = 0.0f;
            }
            
            if (controllerHolder->inputStates) {
                controllerHolder->inputStates[0].stick.x *= 0.05f;
            } 
            
            if (chargeJumpState.chargeTimer[hudSlotId] >= CHARGE_JUMP_MIN_TIME) {
                if (chargeJumpState.hasOriginalScale[hudSlotId]) {
                    kartMovement->scale.y = chargeJumpState.originalScale[hudSlotId].y * CHARGE_JUMP_SINK_SCALE;
                }
                
                if (kartMovement->driftState == 0) {
                    kartMovement->driftState = 1;
                    kartMovement->mtCharge = 0;
                    kartMovement->smtCharge = 0;
                }
                
                const s32 mtMax = 270;
                if (kartMovement->mtCharge < mtMax) {
                    kartMovement->mtCharge += 3;
                    if (kartMovement->mtCharge >= mtMax) {
                        kartMovement->mtCharge = mtMax;
                        kartMovement->driftState = 2;
                    }
                }
                
                kartMovement->outsideDriftAngle = 0.0f;
                kartMovement->conservedTurn = 0.0f;
                kartMovement->effectiveTurn = 0.0f;
                kartMovement->hopStickX = 0;
            }
            
            if (currentSpeed < CHARGE_JUMP_MIN_SPEED) {
                if (chargeJumpState.hasOriginalScale[hudSlotId]) {
                    kartMovement->scale = chargeJumpState.originalScale[hudSlotId];
                }
                
                chargeJumpState.isCharging[hudSlotId] = false;
                chargeJumpState.chargeTimer[hudSlotId] = 0;
                chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
                chargeJumpState.hasOriginalScale[hudSlotId] = false;
            }
        }

        if (!isBButtonHeld && wasButtonHeld && chargeJumpState.isCharging[hudSlotId]) {
            if (chargeJumpState.hasOriginalScale[hudSlotId]) {
                kartMovement->scale = chargeJumpState.originalScale[hudSlotId];
            }
            
            const s32 mtMax = 270;
            bool isMTCharged = kartMovement->mtCharge >= mtMax;
            
            if (chargeJumpState.chargeTimer[hudSlotId] >= CHARGE_JUMP_MIN_TIME && 
                currentSpeed >= CHARGE_JUMP_MIN_SPEED && 
                isMTCharged) {
                UseChargeJumpFeather(itemPlayer);
            }
            
            kartMovement->driftState = 0;
            kartMovement->mtCharge = 0;
            
            chargeJumpState.isCharging[hudSlotId] = false;
            chargeJumpState.chargeTimer[hudSlotId] = 0;
            chargeJumpState.rfhFrameCounter[hudSlotId] = 0;
            chargeJumpState.hasOriginalScale[hudSlotId] = false;
        }

        chargeJumpState.wasHoldingButton[hudSlotId] = isBButtonHeld;
    }
}

static RaceFrameHook ChargeJumpUpdate(UpdateChargeJump);

void DrawChargeJumpDebug() {
    if (!System::sInstance->IsContext(PULSAR_CHARGEJUMP)) {
        return;
    }

    for (u8 hudSlotId = 0; hudSlotId < 4; ++hudSlotId) {
        if (chargeJumpState.isCharging[hudSlotId]) {
            u8 chargeLevel = chargeJumpState.chargeTimer[hudSlotId];
        }
    }
}

}
}   // namespace Pulsar
    // namespace Race