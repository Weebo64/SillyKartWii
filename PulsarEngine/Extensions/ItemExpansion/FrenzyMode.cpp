#include <Extensions/ItemExpansion/FrenzyMode.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/System/Random.hpp>
#include <MarioKartWii/Kart/KartManager.hpp>
#include <MarioKartWii/Kart/KartMovement.hpp>
#include <MarioKartWii/Kart/KartLink.hpp>
#include <MarioKartWii/Kart/KartPhysics.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <SillyKartWii.hpp>
#include <core/rvl/os/OS.hpp>

extern u8 REGION;

extern "C" {
    void DCFlushRange(void* addr, u32 size);
    void ICInvalidateRange(void* addr, u32 size);
}

namespace Pulsar {
namespace Race {

static void EnableTricksAnywhere();
static void DisableTricksAnywhere();

static u32 originalTrickCheck1 = 0;
static u32 originalTrickCheck2 = 0;
static u32 originalTrickJump = 0;
static bool tricksPatched = false;

FrenzyManager* FrenzyManager::sInstance = nullptr;

FrenzyManager* FrenzyManager::CreateInstance() {
    if (!sInstance) {
        sInstance = new FrenzyManager();
    }
    return sInstance;
}

void FrenzyManager::DestroyInstance() {
    if (sInstance) {
        delete sInstance;
        sInstance = nullptr;
    }
}

FrenzyManager::FrenzyManager() {
    Reset();
}

void FrenzyManager::Reset() {
    for (u8 i = 0; i < 12; i++) {
        playerFrenzies[i].Reset(true);
    }
}

bool FrenzyManager::CanItemBeFrenzied(ItemId item) const {
    for (u32 i = 0; i < sizeof(FRENZY_BLACKLIST) / sizeof(ItemId); i++) {
        if (item == FRENZY_BLACKLIST[i]) {
            return false;
        }
    }
    
    if (item >= ITEM_NONE || item < GREEN_SHELL) {
        return false;
    }
    
    return true;
}

u8 FrenzyManager::GetActiveFrenzyCount() const {
    u8 count = 0;
    for (u8 i = 0; i < 12; i++) {
        if (playerFrenzies[i].frenzyFrames > 0) {
            count++;
        }
    }
    return count;
}

bool FrenzyManager::ShouldActivateFrenzy(u8 playerId, bool isHuman) {
    // Disable frenzy mode in Time Trial, Battle modes, and Free Roam
    const Racedata* racedata = Racedata::sInstance;
    if (racedata) {
        const GameMode mode = racedata->racesScenario.settings.gamemode;
        
        // Disable in Time Trial and Ghost Race
        if (mode == MODE_TIME_TRIAL || mode == MODE_GHOST_RACE) {
            return false;
        }
        
        // Disable in all Battle modes
        if (mode == MODE_BATTLE || mode == MODE_PUBLIC_BATTLE || mode == MODE_PRIVATE_BATTLE) {
            return false;
        }
    }
    
    // Disable in Free Roam mode (checking the global free roam flag)
    if (U16_FREE_ROAM != 0x0000) {
        return false;
    }
    
    FrenzyInfo& info = playerFrenzies[playerId];
    
    if (info.forceNextFrenzy) {
        return true;
    }
    
    if (info.totalFrenziesUsed >= MAX_FRENZIES_PER_PLAYER) {
        return false;
    }
    
    if (GetActiveFrenzyCount() >= MAX_SIMULTANEOUS_FRENZIES) {
        return false;
    }
    
    Random random;
    u8 threshold = isHuman ? FRENZY_PROBABILITY_PLAYER : FRENZY_PROBABILITY_CPU;
    u32 roll = random.NextLimited(100);
    
    return roll < threshold;
}

void FrenzyManager::OnItemBoxHit(u8 playerId) {
    if (playerId >= 12) return;
    
    if (playerFrenzies[playerId].frenzyFrames > 0) {
        return;
    }
    
    Item::Manager* mgr = Item::Manager::sInstance;
    if (!mgr || !mgr->players) {
        return;
    }
    
    Item::Player* itemPlayer = &mgr->players[playerId];
    bool isHuman = itemPlayer->isHuman;
    
    if (ShouldActivateFrenzy(playerId, isHuman)) {
        playerFrenzies[playerId].nextItemIsFrenzy = true;
    }
}

void FrenzyManager::OnItemDecided(u8 playerId, ItemId decidedItem) {
    if (playerId >= 12) return;
    
    FrenzyInfo& info = playerFrenzies[playerId];
    
    if (info.nextItemIsFrenzy) {
        if (CanItemBeFrenzied(decidedItem)) {
            info.nextItemIsFrenzy = false;
            info.forceNextFrenzy = false;
            StartFrenzy(playerId, decidedItem);
        } else {
            info.nextItemIsFrenzy = false;
            info.forceNextFrenzy = true;
        }
    }
}

void FrenzyManager::StartFrenzy(u8 playerId, ItemId item) {
    FrenzyInfo& info = playerFrenzies[playerId];
    
    info.frenzyFrames = FRENZY_DURATION;
    info.frenzyItem = item;
    info.totalFrenziesUsed++;
    
    Item::Manager* mgr = Item::Manager::sInstance;
    Kart::Manager* kartMgr = Kart::Manager::sInstance;
    if (mgr && mgr->players) {
        Item::Player* itemPlayer = &mgr->players[playerId];
        itemPlayer->UseStar();
        
        // Extend star duration to 9 seconds (540 frames) like Mario Kart Tour and CTGP-7
        if (kartMgr && kartMgr->players[playerId]) {
            Kart::Link* kartLink = kartMgr->players[playerId];
            Kart::Movement& movement = kartLink->GetMovement();
            movement.starTimer = 540; // 9 seconds at 60 FPS
        }
    }
    
    EnableTricksAnywhere();
}

void FrenzyManager::GiveFrenzyItem(u8 playerId, Item::Player* itemPlayer) {
    if (!itemPlayer) return;
    
    FrenzyInfo& info = playerFrenzies[playerId];
    
    itemPlayer->inventory.SetItem(info.frenzyItem, false);
}

void FrenzyManager::UpdatePlayer(u8 playerId, Item::Player* itemPlayer, Kart::Link* kartLink) {
    if (playerId >= 12 || !itemPlayer) return;
    
    FrenzyInfo& info = playerFrenzies[playerId];
    
    if (info.frenzyFrames == 0) {
        return;
    }
    
    info.frenzyFrames--;
    
    if (info.frenzyFrames == 0) {
        itemPlayer->inventory.currentItemId = ITEM_NONE;
        itemPlayer->inventory.currentItemCount = 0;
        return;
    }
    
    if (kartLink) {
        Kart::PhysicsHolder& physicsHolder = kartLink->GetPhysicsHolder();
        if (physicsHolder.speed.y > 0.0f) {
            physicsHolder.speed.y = 0.0f;
        }
    }
    
    if (itemPlayer->inventory.currentItemId == ITEM_NONE && 
        !itemPlayer->roulette.isTheRouletteSpinning) {
        
        GiveFrenzyItem(playerId, itemPlayer);
    }
}

static ItemId lastPlayerItems[12] = {ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, 
                                     ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
                                     ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE};
static bool lastRouletteStates[12] = {false, false, false, false,
                                      false, false, false, false,
                                      false, false, false, false};

static void EnableTricksAnywhere() {
    if (!tricksPatched) {
        u32 addr1, addr2, addrJump;
        
        if(REGION == 'P') {
            addr1 = 0x80575BC8;
            addr2 = 0x80575BAC;
            addrJump = 0x80575E78;
        } else if(REGION == 'E') {
            addr1 = 0x8056F364;
            addr2 = 0x8056F348;
            addrJump = 0x8056F614;
        } else if(REGION == 'J') {
            addr1 = 0x80575548;
            addr2 = 0x8057552C;
            addrJump = 0x805757F8;
        } else if(REGION == 'K') {
            addr1 = 0x80563C20;
            addr2 = 0x80563C04;
            addrJump = 0x80563ED0;
        } else {
            return;
        }
        
        originalTrickCheck1 = *(u32*)addr1;
        originalTrickCheck2 = *(u32*)addr2;
        originalTrickJump = *(u32*)addrJump;
        
        *(u32*)addr1 = 0x2C00FFFF;
        *(u32*)addr2 = 0x280500FF;
        
        *(u32*)addrJump = 0x54000000;
        
        DCFlushRange((void*)addr1, 4);
        DCFlushRange((void*)addr2, 4);
        DCFlushRange((void*)addrJump, 4);
        ICInvalidateRange((void*)addr1, 4);
        ICInvalidateRange((void*)addr2, 4);
        ICInvalidateRange((void*)addrJump, 4);
        
        tricksPatched = true;
    }
}

static void DisableTricksAnywhere() {
    if (tricksPatched) {
        u32 addr1, addr2, addrJump;
        
        if(REGION == 'P') {
            addr1 = 0x80575BC8;
            addr2 = 0x80575BAC;
            addrJump = 0x80575E78;
        } else if(REGION == 'E') {
            addr1 = 0x8056F364;
            addr2 = 0x8056F348;
            addrJump = 0x8056F614;
        } else if(REGION == 'J') {
            addr1 = 0x80575548;
            addr2 = 0x8057552C;
            addrJump = 0x805757F8;
        } else if(REGION == 'K') {
            addr1 = 0x80563C20;
            addr2 = 0x80563C04;
            addrJump = 0x80563ED0;
        } else {
            return;
        }
        
        *(u32*)addr1 = originalTrickCheck1;
        *(u32*)addr2 = originalTrickCheck2;
        *(u32*)addrJump = originalTrickJump;
        
        DCFlushRange((void*)addr1, 4);
        DCFlushRange((void*)addr2, 4);
        DCFlushRange((void*)addrJump, 4);
        ICInvalidateRange((void*)addr1, 4);
        ICInvalidateRange((void*)addr2, 4);
        ICInvalidateRange((void*)addrJump, 4);
        
        tricksPatched = false;
    }
}

static void FrenzyFrameUpdate() {
    Item::Manager* mgr = Item::Manager::sInstance;
    Kart::Manager* kartMgr = Kart::Manager::sInstance;
    if (!mgr || !mgr->players || !kartMgr || !FrenzyManager::sInstance) return;
    
    for (u8 i = 0; i < 12; i++) {
        Item::Player* itemPlayer = &mgr->players[i];
        if (!itemPlayer) continue;
        
        Kart::Link* kartLink = kartMgr->players[i];
        
        FrenzyManager::sInstance->UpdatePlayer(i, itemPlayer, kartLink);
        
        bool isSpinning = itemPlayer->roulette.isTheRouletteSpinning;
        if (isSpinning && !lastRouletteStates[i]) {
            FrenzyManager::sInstance->OnItemBoxHit(i);
        }
        lastRouletteStates[i] = isSpinning;
        
        ItemId inventoryItem = itemPlayer->inventory.currentItemId;
        
        if (inventoryItem != ITEM_NONE && inventoryItem != lastPlayerItems[i]) {
            FrenzyManager::sInstance->OnItemDecided(i, inventoryItem);
            lastPlayerItems[i] = inventoryItem;
        }
        
        if (inventoryItem == ITEM_NONE) {
            lastPlayerItems[i] = ITEM_NONE;
        }
    }
    
    // UI wird jetzt in FrenzyIndicator.cpp gehandelt
    
    if (FrenzyManager::sInstance->GetActiveFrenzyCount() == 0 && tricksPatched) {
        DisableTricksAnywhere();
    }
}

RaceFrameHook FrenzyUpdate(FrenzyFrameUpdate);

static void CreateFrenzyManager() {
    FrenzyManager::CreateInstance();
}

static void ResetFrenzyOnStart() {
    if (FrenzyManager::sInstance) {
        FrenzyManager::sInstance->Reset();
    }
}

BootHook CreateFrenzyOnBoot(CreateFrenzyManager, 5);
RaceLoadHook ResetFrenzyOnLoad(ResetFrenzyOnStart);

} // namespace Race
} // namespace Pulsar