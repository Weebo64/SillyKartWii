#ifndef _FRENZYMODE_
#define _FRENZYMODE_

#include <kamek.hpp>
#include <MarioKartWii/Item/ItemPlayer.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <MarioKartWii/Kart/KartLink.hpp>
#include <MarioKartWii/UI/Ctrl/UIControl.hpp>
#include <MarioKartWii/UI/Page/RaceHUD/RaceHUD.hpp>

namespace Pulsar {
namespace Race {

static const u32 FRENZY_DURATION = 540;

static const u8 FRENZY_PROBABILITY_PLAYER_12 = 35;
static const u8 FRENZY_PROBABILITY_CPU_12 = 12;

static const u8 FRENZY_PROBABILITY_PLAYER_24 = 10;
static const u8 FRENZY_PROBABILITY_CPU_24 = 6;

static const u32 BMG_FRENZY = 0xD000;

static const u8 MAX_FRENZIES_PER_PLAYER = 10;
static const u8 MAX_SIMULTANEOUS_FRENZIES_12 = 2;
static const u8 MAX_SIMULTANEOUS_FRENZIES_24 = 4;

static const ItemId FRENZY_BLACKLIST[] = {
    BULLET_BILL,
    GOLDEN_MUSHROOM,
    THUNDER_CLOUD,
    POW_BLOCK,
    LIGHTNING,
    BLOOPER
};

struct FrenzyInfo {
    u32 frenzyFrames;
    ItemId frenzyItem;
    u8 totalFrenziesUsed;
    bool nextItemIsFrenzy;
    bool forceNextFrenzy;
    u8 padding[3];
    
    void Reset(bool fullReset) {
        frenzyFrames = 0;
        nextItemIsFrenzy = false;
        if (fullReset) {
            totalFrenziesUsed = 0;
            forceNextFrenzy = false;
            frenzyItem = ITEM_NONE;
        } else {
            if (frenzyFrames > 0) frenzyFrames = 1;
        }
    }
};

class FrenzyManager {
public:
    static FrenzyManager* sInstance;
    static FrenzyManager* CreateInstance();
    static void DestroyInstance();
    
    FrenzyManager();
    
    void OnItemBoxHit(u8 playerId);
    void OnItemDecided(u8 playerId, ItemId decidedItem);
    void UpdatePlayer(u8 playerId, Item::Player* itemPlayer, Kart::Link* kartLink);
    bool CanItemBeFrenzied(ItemId item) const;
    u8 GetActiveFrenzyCount() const;
    
    u8 GetFrenzyProbabilityPlayer() const;
    u8 GetFrenzyProbabilityCPU() const;
    u8 GetMaxSimultaneousFrenzies() const;
    
    bool IsFrenzyActive(u8 playerId) const {
        return playerFrenzies[playerId].frenzyFrames > 0;
    }
    
    u32 GetFrenzyFramesRemaining(u8 playerId) const {
        if (playerId < 24) {
            return playerFrenzies[playerId].frenzyFrames;
        }
        return 0;
    }
    
    ItemId GetFrenzyItem(u8 playerId) const {
        if (playerId < 24) {
            return playerFrenzies[playerId].frenzyItem;
        }
        return ITEM_NONE;
    }
    
    void ForceNextFrenzy(u8 playerId) {
        if (playerId < 24) {
            playerFrenzies[playerId].forceNextFrenzy = true;
        }
    }
    
    void Reset();
    
private:
    FrenzyInfo playerFrenzies[24];
    
    void StartFrenzy(u8 playerId, ItemId item);
    bool ShouldActivateFrenzy(u8 playerId, bool isHuman);
    void GiveFrenzyItem(u8 playerId, Item::Player* itemPlayer);
};

} // namespace Race
} // namespace Pulsar

#endif
