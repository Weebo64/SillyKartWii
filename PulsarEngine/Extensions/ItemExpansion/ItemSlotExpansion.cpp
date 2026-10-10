/*
 * ItemSlotExpansion.cpp
 * 
 * Extends the item system from 19 to 27 slots for future expansion
 * Currently configured for VANILLA ITEMS ONLY
 * 
 * This file:
 * - Creates an expanded 27-slot behavior table
 * - Patches all game functions to use the expanded table
 * - Reserves slots 19-26 for future custom items (currently inactive)
 * 
 * Based on Insane-Kart-Wii implementation by SaucyCF
 */

#include <kamek.hpp>
#include <MarioKartWii/Item/ItemBehaviour.hpp>
#include <MarioKartWii/Item/ItemPlayer.hpp>
#include <MarioKartWii/Item/ItemSlot.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/Kart/KartManager.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <Extensions/ItemExpansion/ItemProbabilities.hpp>
#include <Network/PacketExpansion.hpp>
#include <MarioKartWii/Driver/DriverManager.hpp>

static const u32 ORIGINAL_ITEM_COUNT = 19;
static const u32 EXPANDED_ITEM_COUNT = 27;

// Expanded behavior table and accumulator in BSS section
extern "C" {
    Item::Behavior expandedBehaviourTable[27];
    u16 expandedAccumulator[27];
}

// Allocate zeroed memory for item slot data
extern "C" void* __nwa__FUl(u32 size);

static void* AllocZeroed(u32 size) {
    void* ptr = __nwa__FUl(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}
kmCall(0x807baa28, AllocZeroed);

// Binary item slot compilation (for loading item probabilities)
extern "C" {
    u8 compiledItemSlotBin[3000];
    u32 compiledItemSlotLen = 0;
}

extern "C" void BuildItemSlotBinary() {
    u32 offset = 0;
    compiledItemSlotBin[offset++] = (u8)ItemProbs::TABLE_COUNT;
    for (u32 t = 0; t < ItemProbs::TABLE_COUNT; t++) {
        const ItemProbs::TableDef& def = ItemProbs::ALL_TABLES[t];
        u32 cols = def.cols;
        compiledItemSlotBin[offset++] = (u8)cols;
        compiledItemSlotBin[offset++] = (u8)ItemProbs::ITEM_COUNT;
        u32 dataSize = ItemProbs::ITEM_COUNT * cols;
        memcpy(&compiledItemSlotBin[offset], def.data, dataSize);
        offset += dataSize;
    }
    compiledItemSlotLen = offset;
}

// =============================================================================
// ASM HOOKS - Replace vanilla behavior table with expanded table
// =============================================================================

asmFunc DecideItemTableHook() {
    ASM(
    nofralloc;
    lis r26, expandedBehaviourTable @ha;
    addi r26, r26, expandedBehaviourTable @l;
    lis r16, expandedAccumulator @ha;
    addi r16, r16, expandedAccumulator @l;
    li r21, 0;
    li r17, 0;
    blr;
    )
}
kmBranch(0x807bb6b4, DecideItemTableHook);
kmPatchExitPoint(DecideItemTableHook, 0x807bb6cc);

asmFunc AccumulatorReadHook() {
    ASM(
    nofralloc;
    lis r4, expandedAccumulator @ha;
    rlwinm r3, r3, 0, 16, 31;
    addi r4, r4, expandedAccumulator @l;
    blr;
    )
}
kmBranch(0x807bb868, AccumulatorReadHook);
kmPatchExitPoint(AccumulatorReadHook, 0x807bb874);

asmFunc UseItemTableHook() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmBranch(0x80791960, UseItemTableHook);
kmPatchExitPoint(UseItemTableHook, 0x80791968);

asmFunc DefineCapacitiesTableHook() {
    ASM(
    nofralloc;
    lis r5, expandedBehaviourTable @ha;
    addi r5, r5, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80790fcc, 0x60000000);
kmBranch(0x80790fdc, DefineCapacitiesTableHook);
kmPatchExitPoint(DefineCapacitiesTableHook, 0x80790fe0);

asmFunc ProcessTableTableHook() {
    ASM(
    nofralloc;
    lis r23, expandedBehaviourTable @ha;
    addi r23, r23, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807baa3c, 0x60000000);
kmBranch(0x807baa44, ProcessTableTableHook);
kmPatchExitPoint(ProcessTableTableHook, 0x807baa48);

asmFunc CanSpawnTableHook() {
    ASM(
    nofralloc;
    lis r4, expandedBehaviourTable @ha;
    addi r4, r4, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80799b34, 0x60000000);
kmBranch(0x80799b40, CanSpawnTableHook);
kmPatchExitPoint(CanSpawnTableHook, 0x80799b44);

asmFunc DropItemTableHook1() {
    ASM(
    nofralloc;
    lis r4, expandedBehaviourTable @ha;
    addi r4, r4, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc3f8, 0x60000000);
kmBranch(0x807bc400, DropItemTableHook1);
kmPatchExitPoint(DropItemTableHook1, 0x807bc404);

asmFunc DropItemTableHook2() {
    ASM(
    nofralloc;
    lis r8, expandedBehaviourTable @ha;
    addi r8, r8, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc454, 0x60000000);
kmBranch(0x807bc468, DropItemTableHook2);
kmPatchExitPoint(DropItemTableHook2, 0x807bc46c);

asmFunc InventoryInitTableHook() {
    ASM(
    nofralloc;
    lis r7, expandedBehaviourTable @ha;
    addi r7, r7, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc90c, 0x60000000);
kmBranch(0x807bc918, InventoryInitTableHook);
kmPatchExitPoint(InventoryInitTableHook, 0x807bc91c);

asmFunc SetItemTableHook() {
    ASM(
    nofralloc;
    lis r6, expandedBehaviourTable @ha;
    addi r6, r6, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc944, 0x60000000);
kmBranch(0x807bc950, SetItemTableHook);
kmPatchExitPoint(SetItemTableHook, 0x807bc954);

asmFunc UpdateItemArraysTableHook() {
    ASM(
    nofralloc;
    lis r22, expandedBehaviourTable @ha;
    addi r22, r22, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x8065f778, 0x60000000);
kmBranch(0x8065f788, UpdateItemArraysTableHook);
kmPatchExitPoint(UpdateItemArraysTableHook, 0x8065f78c);

asmFunc UpdateItemSumsTableHook() {
    ASM(
    nofralloc;
    lis r31, expandedBehaviourTable @ha;
    addi r31, r31, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x8065e0bc, 0x60000000);
kmBranch(0x8065e0d8, UpdateItemSumsTableHook);
kmPatchExitPoint(UpdateItemSumsTableHook, 0x8065e0dc);

asmFunc SpawnLimitTableHook() {
    ASM(
    nofralloc;
    lis r4, expandedBehaviourTable @ha;
    addi r4, r4, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80799c08, 0x60000000);
kmBranch(0x80799c10, SpawnLimitTableHook);
kmPatchExitPoint(SpawnLimitTableHook, 0x80799c14);

asmFunc BreakableObjectTableHook() {
    ASM(
    nofralloc;
    lis r4, expandedBehaviourTable @ha;
    addi r4, r4, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x8076ef58, 0x60000000);
kmBranch(0x8076ef60, BreakableObjectTableHook);
kmPatchExitPoint(BreakableObjectTableHook, 0x8076ef64);

asmFunc GetItemCountTableHook1() {
    ASM(
    nofralloc;
    lis r5, expandedBehaviourTable @ha;
    addi r5, r5, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80798f20, 0x60000000);
kmBranch(0x80798f24, GetItemCountTableHook1);
kmPatchExitPoint(GetItemCountTableHook1, 0x80798f28);

asmFunc GetItemCountTableHook2() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80798f58, 0x60000000);
kmBranch(0x80798f5c, GetItemCountTableHook2);
kmPatchExitPoint(GetItemCountTableHook2, 0x80798f60);

asmFunc AIItemTableHook() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80733c40, 0x60000000);
kmBranch(0x80733c44, AIItemTableHook);
kmPatchExitPoint(AIItemTableHook, 0x80733c48);

asmFunc UseTypeQueryTableHook() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x80798270, 0x60000000);
kmBranch(0x80798274, UseTypeQueryTableHook);
kmPatchExitPoint(UseTypeQueryTableHook, 0x80798278);

asmFunc InventoryTickTableHook1() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc7c0, 0x60000000);
kmBranch(0x807bc7c4, InventoryTickTableHook1);
kmPatchExitPoint(InventoryTickTableHook1, 0x807bc7c8);

asmFunc InventoryTickTableHook2() {
    ASM(
    nofralloc;
    lis r3, expandedBehaviourTable @ha;
    addi r3, r3, expandedBehaviourTable @l;
    blr;
    )
}
kmWrite32(0x807bc898, 0x60000000);
kmBranch(0x807bc89c, InventoryTickTableHook2);
kmPatchExitPoint(InventoryTickTableHook2, 0x807bc8a0);

// =============================================================================
// SIZE PATCHES - Update constants for 27-slot table
// =============================================================================

// Item::ItemSlotData::Init (0x807baf48)
kmWrite32(0x807baf5c, 0x38600070);  // sizeof(u16) * 27 + 2 alignment = 0x38
kmWrite32(0x807baf80, 0x38600550);  // sizeof(Behavior) * 27 * 12 tables
kmWrite32(0x807baf94, 0x38C00070);  // Size for allocation
kmWrite32(0x807bb284, 0x1C7D0070);  // Size multiplier
kmWrite32(0x807bb2a0, 0x38C00070);  // Size for copy
kmWrite32(0x807bb338, 0x2C0A001B); // Compare with 27 items
kmWrite32(0x807bb340, 0x38C60036); // +0x36 offset (27*2)
kmWrite32(0x807bb344, 0x38E70070); // +0x70 offset

// ProcessTable (0x807ba9d8)
kmWrite32(0x807baa20, 0x1C7F0036); // 27 * 2
kmWrite32(0x807bacc0, 0x38C60036); // +0x36
kmWrite32(0x807bacc4, 0x38E70070); // +0x70

// DecideItem (0x807bb42c)
kmWrite32(0x807bb620, 0x1C640036); // 27 * 2
kmWrite32(0x807bb6a8, 0x3B60001B); // li r27, 27
kmWrite32(0x807bb6b0, 0x3B60001B); // li r27, 27

// calcRandomItem / DecideRouletteItem (0x807bb8d0)
kmWrite32(0x807bb8f8, 0x1C000070); // sizeof(Behavior) * 27
kmWrite32(0x807bb954, 0x1C000070); // sizeof(Behavior) * 27

// PostProcessVSTable / scaleTable (0x807bad20)
kmWrite32(0x807bad5c, 0x1C000036); // 27 * 2
kmWrite32(0x807bae0c, 0x1C890036); // 27 * 2

// Scale table copy/clear hooks for expanded slots
asmFunc ScaleTableCopyHook() {
    ASM(
    nofralloc;
    // Copy bytes for slots 19-26 (8 extra slots)
    lhz r0, 0x26(r4);
    sth r0, 0x26(r7);
    lhz r0, 0x28(r4);
    sth r0, 0x28(r7);
    lhz r0, 0x2A(r4);
    sth r0, 0x2A(r7);
    lhz r0, 0x2C(r4);
    sth r0, 0x2C(r7);
    lhz r0, 0x2E(r4);
    sth r0, 0x2E(r7);
    lhz r0, 0x30(r4);
    sth r0, 0x30(r7);
    lhz r0, 0x32(r4);
    sth r0, 0x32(r7);
    lhz r0, 0x34(r4);
    sth r0, 0x34(r7);
    addi r7, r7, 0x36;
    blr;
    )
}
kmBranch(0x807badfc, ScaleTableCopyHook);
kmPatchExitPoint(ScaleTableCopyHook, 0x807bae00);

asmFunc ScaleTableClearHook() {
    ASM(
    nofralloc;
    // Clear bytes for slots 19-26
    sth r3, 0x26(r4);
    sth r3, 0x28(r4);
    sth r3, 0x2A(r4);
    sth r3, 0x2C(r4);
    sth r3, 0x2E(r4);
    sth r3, 0x30(r4);
    sth r3, 0x32(r4);
    sth r3, 0x34(r4);
    addi r4, r4, 0x36;
    blr;
    )
}
kmBranch(0x807bae74, ScaleTableClearHook);
kmPatchExitPoint(ScaleTableClearHook, 0x807bae78);

// =============================================================================
// INITIALIZATION
// =============================================================================

static void InitExpandedItemBehaviours() {
    // Copy vanilla behaviors (slots 0-18)
    memcpy(expandedBehaviourTable, Item::Behavior::behaviourTable, 
           ORIGINAL_ITEM_COUNT * sizeof(Item::Behavior));
    
    // Zero out reserved slots (19-20)
    memset(&expandedBehaviourTable[UNKNOWN_0x13], 0, sizeof(Item::Behavior));
    memset(&expandedBehaviourTable[ITEM_NONE], 0, sizeof(Item::Behavior));
    
    // Clear accumulator
    memset(expandedAccumulator, 0, sizeof(expandedAccumulator));
}
RaceLoadHook InitExpanded(InitExpandedItemBehaviours);

// =============================================================================
// ITEM CAPACITY CHECK (NO BOO/FEATHER SUPPORT)
// =============================================================================

// Simple capacity check - always returns true for now (no custom items active)
extern "C" bool ExpandedCapacityCheck(ItemId id) {
    return Item::Manager::IsThereCapacityForItem(id);
}
kmCall(0x8072fda4, ExpandedCapacityCheck);
kmCall(0x807ba17c, ExpandedCapacityCheck);

