/*
 * ItemProbabilities.hpp
 * 
 * Item probability tables for SillyKartWii
 * Extended to support 27 item slots (vanilla uses 19)
 * 
 * Currently configured for VANILLA ITEMS ONLY (slots 0-18)
 * Slots 19-26 are reserved for future expansion
 *
 * FORMAT:
 *   Each entry is a raw probability weight (u8, 0-200 scale).
 *   Column sums should total approximately 200 (= 100% probability).
 *   A value of 200 in a column makes that item guaranteed for that position.
 *
 * RACE TABLES  (12 columns): col 0 = 1st place, col 1 = 2nd, ... col 11 = 12th
 * BATTLE TABLES (3 columns): col 0 = tied/lead, col 1 = behind tier 1, col 2 = behind tier 2
 * SPECIAL TABLE (16 columns): each column = a specific Special Box variant
 *
 * ITEM INDICES (VANILLA):
 *    0 = Green Shell         7 = Blue Shell        14 = Thundercloud
 *    1 = Red Shell           8 = Lightning         15 = Bullet Bill
 *    2 = Banana              9 = Star              16 = Triple Green Shells
 *    3 = Fake Item Box      10 = Golden Mushroom   17 = Triple Red Shells
 *    4 = Mushroom           11 = Mega Mushroom     18 = Triple Bananas
 *    5 = Triple Mushroom    12 = Blooper
 *    6 = Bob-omb            13 = POW Block
 *
 * RESERVED FOR FUTURE (CURRENTLY INACTIVE):
 *   19 = UNKNOWN_0x13 (unused)
 *   20 = ITEM_NONE (unused)
 *   21-26 = Reserved for custom items
 */

#pragma once
#include <types.hpp>

namespace ItemProbs {

static const u32 ITEM_COUNT = 27;  // Extended from vanilla 19
static const u32 TABLE_COUNT = 12;

// VANILLA ITEM PROBABILITIES - Unchanged from base game
//     1st  2nd  3rd  4th  5th  6th  7th  8th  9th 10th 11th 12th
static const u8 GLOBAL_ITEM_PROB_RACE[ITEM_COUNT][12] = {
    {  50,  35,  20,  10,   0,   0,   0,   0,   0,   0,   0,   0 }, //  0: Green Shell
    {  10,  50,  40,  25,  15,  10,   5,   0,   0,   0,   0,   0 }, //  1: Red Shell
    {  75,  35,  20,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  2: Banana
    {  35,  20,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  3: Fake Item Box
    {   0,  15,  25,  15,  10,   5,   0,   0,   0,   0,   0,   0 }, //  4: Mushroom
    {   0,   0,  10,  25,  35,  40,  40,  45,  50,  60,  40,  10 }, //  5: Triple Mushroom
    {   0,   5,  15,  20,  20,  15,  10,   5,   0,   0,   0,   0 }, //  6: Bob-omb
    {   0,   0,   0,   5,  10,  15,  15,  10,   5,   0,   0,   0 }, //  7: Blue Shell
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  25 }, //  8: Lightning
    {   0,   0,   0,   0,   0,   0,   0,  10,  15,  50,  75,  50 }, //  9: Star
    {   0,   0,   0,   0,   0,   0,   0,   5,  20,  45,  55,  70 }, // 10: Golden Mushroom
    {   0,   0,   0,  10,  15,  15,  20,  30,  35,  20,   0,   0 }, // 11: Mega Mushroom
    {   0,   0,   0,   5,  10,  10,  15,  10,   0,   0,   0,   0 }, // 12: Blooper
    {   0,   0,   0,   0,   5,  10,  10,  10,   5,   0,   0,   0 }, // 13: POW Block
    {   0,   0,  10,  15,  15,  15,  15,  10,  10,   0,   0,   0 }, // 14: Thundercloud
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  25,  45 }, // 15: Bullet Bill
    {   0,  10,  20,  25,  20,  15,  10,   5,   0,   0,   0,   0 }, // 16: Triple Green Shells
    {   0,   0,   0,  10,  15,  15,  15,  15,  25,   5,   0,   0 }, // 17: Triple Red Shells
    {  25,  20,  15,  10,   5,   0,   0,   0,   0,   0,   0,   0 }, // 18: Triple Bananas
    // RESERVED SLOTS - All zeros (inactive)
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 19: UNKNOWN_0x13
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 20: ITEM_NONE
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 21: Reserved
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 22: Reserved
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 23: Reserved
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 24: Reserved
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 25: Reserved
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 26: Reserved
};

static const u8 SPECIAL[ITEM_COUNT][16] = {
    {   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  0: Green Shell
    {   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0 }, //  1: Red Shell
    { 200,   0,   0,   0,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  2: Banana
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  3: Fake Item Box
    {   0, 200,   0,   0,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  4: Mushroom
    {   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  5: Triple Mushroom
    {   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0 }, //  6: Bob-omb
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  7: Blue Shell
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  8: Lightning
    {   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, //  9: Star
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 10: Golden Mushroom
    {   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0 }, // 11: Mega Mushroom
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 12: Blooper
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 13: POW Block
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0 }, // 14: Thundercloud
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 15: Bullet Bill
    {   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 16: Triple Green Shells
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 17: Triple Red Shells
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 18: Triple Bananas
    // RESERVED SLOTS
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 19-26
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
};

static const u8 GLOBAL_ITEM_PROB_BATTLE[ITEM_COUNT][3] = {
    {  50,  15,   0 }, //  0: Green Shell
    {  50,  60,  60 }, //  1: Red Shell
    {  25,  10,   0 }, //  2: Banana
    {  15,  10,   0 }, //  3: Fake Item Box
    {  15,  10,   0 }, //  4: Mushroom
    {   0,  10,  20 }, //  5: Triple Mushroom
    {  10,  10,  10 }, //  6: Bob-omb
    {   0,   0,  10 }, //  7: Blue Shell
    {   0,   0,  10 }, //  8: Lightning
    {   0,  10,  20 }, //  9: Star
    {   0,   0,   0 }, // 10: Golden Mushroom
    {  10,  15,  20 }, // 11: Mega Mushroom
    {   0,  10,   5 }, // 12: Blooper
    {   0,   0,   0 }, // 13: POW Block
    {   0,   0,   0 }, // 14: Thundercloud
    {   0,   0,   0 }, // 15: Bullet Bill
    {  15,  15,  15 }, // 16: Triple Green Shells
    {   0,  15,  30 }, // 17: Triple Red Shells
    {  10,  10,   0 }, // 18: Triple Bananas
    // RESERVED SLOTS
    {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
    {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
};

// Table definition structure
struct TableDef {
    const u8* data;
    u32 cols;
};

// All table definitions in the order the game expects
static const TableDef ALL_TABLES[TABLE_COUNT] = {
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 0: GRAND_PRIX_PLAYER
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 1: GRAND_PRIX_ENEMY
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 2: VERSUS_PLAYER
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 3: VERSUS_ENEMY
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 4: VERSUS_ONLINE
    { &SPECIAL[0][0],                 16 },  // 5: SPECIAL
    { &GLOBAL_ITEM_PROB_BATTLE[0][0],  3 },  // 6: BALLOON_PLAYER
    { &GLOBAL_ITEM_PROB_BATTLE[0][0],  3 },  // 7: BALLOON_ENEMY
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 8: COIN_PLAYER
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 9: COIN_ENEMY
    { &GLOBAL_ITEM_PROB_BATTLE[0][0],  3 },  // 10: BALLOON_ONLINE
    { &GLOBAL_ITEM_PROB_RACE[0][0],   12 },  // 11: COIN_ONLINE
};

} // namespace ItemProbs
