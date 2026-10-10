# Frenzy Mode for SillyKartWii

## Overview
Frenzy Mode is a Mario Kart Tour-inspired feature ported from CTGP-7 (Mario Kart 7). When activated, players gain a Star effect and automatically refill their item for approximately 7.5 seconds.

## Features

### Core Mechanics
- **Duration**: 450 frames (~7.5 seconds at 60 FPS)
- **Effect**: Star invincibility + automatic item respawning
- **Probability**:
  - Human players: 20% chance per item box
  - CPU players: 10% chance per item box
- **Limits**:
  - Max 3 frenzies per player per race
  - Max 2 simultaneous active frenzies in the race

### Blacklisted Items
The following items cannot trigger Frenzy:
- Bullet Bill (too overpowered)
- Golden Mushroom (already has repeat functionality)
- Thundercloud (annoying)
- POW Block (global effect)
- Lightning (global effect)
- Blooper (annoying in frenzy)

If a blacklisted item is pulled when Frenzy should activate, the Frenzy is delayed to the next item box.

## Technical Implementation

### Files
- `FrenzyMode.hpp` - Header with structs and class definition
- `FrenzyMode.cpp` - Implementation with Kamek hooks

### Key Components

#### FrenzyInfo Struct
Stores per-player frenzy data:
```cpp
struct FrenzyInfo {
    u32 frenzyFrames;           // Countdown timer
    ItemId frenzyItem;          // Item to respawn
    u8 totalFrenziesUsed;       // Total frenzies this race
    bool nextItemIsFrenzy;      // Trigger flag
    bool forceNextFrenzy;       // Force frenzy on valid item
};
```

#### FrenzyManager Class
Singleton manager that handles:
- Item box hit detection
- Frenzy activation logic
- Per-frame updates and item respawning
- Lifecycle management

### Hooks
The system uses Kamek ASM hooks at these addresses:
- `0x80798C38` - Item::Player::DecideItem (detects item box hits)
- `0x807ba374` - PlayerRoulette::OnRouletteEnd (detects item decision)
- `0x8079792C` - Item::Player::Update (frame updates)
- `0x80533104` - RaceInfo::Init (race start reset)

## Current Status

### ✅ Implemented
- [x] Frenzy activation system with probability checks
- [x] Star effect activation
- [x] Automatic item respawning
- [x] Item blacklist
- [x] Per-player limits (max 3 per race)
- [x] Global limits (max 2 simultaneous)
- [x] Frame-based countdown system
- [x] Race lifecycle management

### ❌ Not Implemented (Future)
- [ ] UI/HUD feedback (audio via Star music only - visual text not working yet)
  - DirectPrint doesn't work during normal gameplay
  - ghostMessage approach didn't display text
  - Need proper .brctr layout file or alternative text display method
- [ ] Visual "FRENZY" text overlay (attempted but non-functional)
- [ ] Custom sound effects (currently uses Star music)
- [ ] Online synchronization (netcode)
- [ ] Settings/configuration options

## Usage

### For Players
Frenzy activates automatically when you hit an item box (20% chance). You'll know it's active because:
- Star music plays
- You have invincibility (Star effect)
- Your item automatically refills when used

### For Developers

#### Enable/Disable
The system is automatically active when the files are compiled. To disable, comment out the BootHook in `FrenzyMode.cpp`.

#### Adjust Probabilities
Edit constants in `FrenzyMode.hpp`:
```cpp
static const u8 FRENZY_PROBABILITY_PLAYER = 20;  // Change to 0-100
static const u8 FRENZY_PROBABILITY_CPU = 10;
```

#### Force Frenzy (Testing)
Call from code:
```cpp
FrenzyManager::sInstance->ForceNextFrenzy(playerId);
```

#### Add/Remove Blacklisted Items
Edit the `FRENZY_BLACKLIST` array in `FrenzyMode.hpp`.

## Compilation
Add to your Kamek project's sources list:
```
PulsarEngine/Extensions/ItemExpansion/FrenzyMode.cpp
```

Make sure `ItemExpansion` folder is in your include path.

## Credits
- **Original Concept**: Mario Kart Tour (Nintendo)
- **CTGP-7 Implementation**: PabloMK7 and CTGP-7 team
- **MKWii Port**: SillyKartWii team

## Known Issues
- No UI feedback (only audio via Star)
- Not tested in online multiplayer
- May need netcode synchronization for online play

## Future Enhancements
1. **UI System**: Add HUD overlay with "FRENZY!" text and timer bar
2. **Sounds**: Custom frenzy start/end sound effects
3. **Online Support**: Netcode for frenzy synchronization
4. **Settings**: Make probability configurable in-game
5. **Visual Effects**: Additional particle effects or kart glow during frenzy


## UI Implementation Notes

### What Works
- ✅ Frenzy Mode core functionality (Star effect, item respawn, tricks anywhere)
- ✅ Audio feedback (Star music plays during Frenzy)
- ✅ Frame-based countdown system
- ✅ All game logic and timing

### What Doesn't Work (Yet)
- ❌ Visual "FRENZY!" text display

### Why UI Doesn't Work
Several approaches were attempted:

1. **DirectPrint_DrawString** - Doesn't render during normal gameplay (only works for exception/crash screens)
2. **RaceHUD ghostMessage** - Text doesn't appear (possibly timing/initialization issue)
3. **LayoutUIControl creation** - Needs proper .brctr layout file (crashes without it)

### Future UI Solutions

**Option 1: Proper Layout File (Recommended)**
- Create `frenzy_indicator.brctr` layout file with CTLib or similar tools
- Implement CtrlRaceFrenzyIndicator similar to Speedometer
- Most correct approach but requires layout editing

**Option 2: Hijack Existing UI**
- Temporarily replace speedometer text with "FRENZY!"
- Modify item window appearance (glow, pulse, color change)
- Simpler but less clean

**Option 3: Sound-Only**
- Play distinct sound effect when Frenzy starts/ends
- No visual needed if audio is enough
- Easiest to implement

**Option 4: Console/Gecko Code Text**
- Use Gecko-style on-screen display
- Requires specific Dolphin/console setup
- Works for testing but not production

For now, **Frenzy Mode is fully functional** - you can tell when it's active by:
- Star music playing
- Invincibility (Star effect)
- Items automatically respawning
- Ability to trick anywhere

The visual "FRENZY!" text would be nice-to-have but isn't essential for gameplay.
