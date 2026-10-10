#include <kamek.hpp>
#include <MarioKartWii/Kart/KartCollision.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/Item/ItemBehaviour.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <PulsarSystem.hpp>

namespace Pulsar {
namespace Race {

static bool IsAllItemsCanLandActive() {
    System* system = System::sInstance;
    if(!system) return false;
    
    // Check if ItemRain is active
    if(system->IsContext(PULSAR_ITEMMODERAIN)) {
        const RKNet::RoomType roomType = RKNet::Controller::sInstance->roomType;
        if (roomType == RKNet::ROOMTYPE_FROOM_HOST ||
            roomType == RKNet::ROOMTYPE_FROOM_NONHOST ||
            roomType == RKNet::ROOMTYPE_NONE ||
            roomType == RKNet::ROOMTYPE_VS_REGIONAL ||
            roomType == RKNet::ROOMTYPE_JOINING_REGIONAL) {
            return true;
        }
    }
    
    // Check if CT is active
    return system->IsContext(PULSAR_CT);
}

int UseItem(Kart::Collision *kartCollision, ItemId id){
    u8 playerId = kartCollision->GetPlayerIdx();
    Item::Manager::sInstance->players[playerId].inventory.currentItemCount++;
    Item::Behavior::behaviourTable[id].useFunction(Item::Manager::sInstance->players[playerId]);
    return -1;
}

int AllShocksCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, LIGHTNING);
    return -1;
}

int AllMegasCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, MEGA_MUSHROOM);
    return -1;
}

int AllFeathersCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, BLOOPER);
    return -1;
}

int AllPOWsCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, POW_BLOCK);
    return -1;
}

int AllGoldensCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, MUSHROOM);
    return -1;
}

int AllBulletsCanLand(Kart::Collision *kartCollision){
    if (IsAllItemsCanLandActive()) return UseItem(kartCollision, BULLET_BILL);
    return -1;
}

kmWritePointer(0x808b54b8, AllShocksCanLand);
kmWritePointer(0x808b54d0, AllMegasCanLand);
kmWritePointer(0x808b54f4, AllPOWsCanLand);
kmWritePointer(0x808b5500, AllGoldensCanLand);
kmWritePointer(0x808b550c, AllBulletsCanLand);
kmWrite32(0x807A66C4, 0x60000000);
kmWrite32(0x80796D30, 0x38600000);
kmWrite32(0x80790EF0, 0x39800001);
kmWrite32(0x80790EF4, 0x39600001);
kmWrite32(0x80790EF8, 0x39400001);
kmWrite32(0x80790EFC, 0x39200001);
//kmWritePointer(0x808b54e8, AllFeathersCanLand); Already done in Pulsar
} // namespace Race
} // namespace Pulsar
