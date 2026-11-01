// Random Events during races by Weebo64
// random effects every 5-15 seconds

#include <kamek.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/Race/RaceInfo/RaceInfo.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/Item/Obj/ItemObj.hpp>
#include <MarioKartWii/Kart/KartPlayer.hpp>
#include <MarioKartWii/Kart/KartManager.hpp>
#include <MarioKartWii/Kart/KartMovement.hpp>
#include <PulsarSystem.hpp>
#include <SillyKartWii.hpp>

namespace Pulsar {
namespace SKWii {

static s32 sRaceInfoFrameCounter = 0;
static s32 sNextEventFrame = 0;
static u8 sCurrentEventType = 0xFF;
static s16 sEventDuration = 0;

static u32 GetRandom() {
    static u32 counter = 0;
    counter++;
    u32 value = (sRaceInfoFrameCounter * 1103515245 + counter * 12345) ^ (counter << 8);
    return value * 1664525 + 1013904223;
}

static void Event_MegaMode() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->ActivateMega();
        }
    }
    sEventDuration = 540;
}

static void Event_MiniMode() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->ApplyLightning();
        }
    }
    sEventDuration = 540;
}

static void Event_SpeedBoostAll() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->boost.Activate(2, 180);
        }
    }
    sEventDuration = 180;
}

static void Event_SpinOut() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->ApplyLightning();
        }
    }
    sEventDuration = 0;
}

static void Event_TeleportShuffle() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    if (playerCount < 2) return;
    
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->DoRespawn();
        }
    }
    
    sEventDuration = 0;
}

static void Event_BackwardsRace() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->ApplyInk(false);
        }
    }
    sEventDuration = 300;
}

static void Event_FlyingKarts() {
    if (!Kart::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Kart::Player* kartPlayer = Kart::Manager::sInstance->players[i];
        if (!kartPlayer) continue;
        
        Kart::Movement* movement = kartPlayer->pointers.kartMovement;
        if (movement) {
            movement->hopVelY = 80.0f;
        }
    }
    sEventDuration = 0;
}

static ItemObjId GetRandomItemId() {
    ItemObjId items[] = {
        OBJ_MUSHROOM,
        OBJ_GREEN_SHELL,
        OBJ_BANANA,
        OBJ_RED_SHELL,
        OBJ_BOBOMB,
        OBJ_STAR
    };
    return items[GetRandom() % 6];
}

static void Event_ItemStorm() {
    if (!Item::Manager::sInstance) return;
    
    u8 playerCount = Pulsar::System::sInstance->nonTTGhostPlayersCount;
    for (u8 i = 0; i < playerCount && i < 12; i++) {
        Item::Player& player = Item::Manager::sInstance->players[i];
        if (!player.kartPlayer) continue;
        
        Vec3 playerPos = player.GetPosition();
        Vec3 dummyDir;
        dummyDir.x = 0.0f;
        dummyDir.y = 0.0f;
        dummyDir.z = 0.0f;
        
        for (int j = 0; j < 3; j++) {
            Vec3 spawnPos = playerPos;
            spawnPos.y += 200.0f + (j * 100.0f);
            ItemObjId itemId = GetRandomItemId();
            Item::Manager::sInstance->CreateItemDirect(itemId, &spawnPos, &dummyDir, i);
        }
    }
    sEventDuration = 0;
}

static void TriggerRandomEvent() {
    u8 eventType = GetRandom() % 8;
    sCurrentEventType = eventType;
    
    switch (eventType) {
        case 0:
            Event_MegaMode();
            break;
        case 1:
            Event_MiniMode();
            break;
        case 2:
            Event_SpeedBoostAll();
            break;
        case 3:
            Event_SpinOut();
            break;
        case 4:
            Event_TeleportShuffle();
            break;
        case 5:
            Event_BackwardsRace();
            break;
        case 6:
            Event_FlyingKarts();
            break;
        case 7:
            Event_ItemStorm();
            break;
        default:
            Event_SpeedBoostAll();
            break;
    }
    
    u32 minWait = 180;
    u32 maxWait = 1200;
    sNextEventFrame = sRaceInfoFrameCounter + minWait + (GetRandom() % (maxWait - minWait + 1));
}

static void CleanupEvent() {
    sCurrentEventType = 0xFF;
    sEventDuration = 0;
}

void UpdateRandomEvents() {
    if (U16_RANDOMEVENTS != 0x0001) return;
    if (!Racedata::sInstance || !Raceinfo::sInstance || !Kart::Manager::sInstance) return;
    if (!Raceinfo::sInstance->IsAtLeastStage(RACESTAGE_RACE)) return;
    
    sRaceInfoFrameCounter++;
    
    if (sNextEventFrame == 0) {
        u32 minWait = 180;
        u32 maxWait = 1200;
        sNextEventFrame = sRaceInfoFrameCounter + minWait + (GetRandom() % (maxWait - minWait + 1));
    }
    
    if (sEventDuration > 0) {
        sEventDuration--;
        if (sEventDuration == 0) {
            CleanupEvent();
        }
    }
    
    if (sRaceInfoFrameCounter >= sNextEventFrame) {
        TriggerRandomEvent();
    }
}

RaceFrameHook RandomEventsHook(UpdateRandomEvents);

} // namespace SKWii
} // namespace Pulsar
