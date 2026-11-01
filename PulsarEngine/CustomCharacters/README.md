# Custom Characters System (from Retro Rewind)

This folder contains the complete Retro Rewind custom character system, which has been integrated into SillyKartWii.

## Features

- **Custom Character Skins**: Load custom driver BRRES models with up to 50 skins per character
- **Custom Animations**: Support for special animations (shockHit, starUse, megaUse, waitBeforeStart, shockDodgeStar)
- **Custom Voices**: Loose voice file support for custom characters
- **Custom Sound Effects**: Override character sound effects
- **Menu Integration**: Character select UI with skin cycling (L/R buttons)
- **Online Support**: Custom skins work in online races
- **CPU Skins**: Offline CPU racers get random custom skins
- **BMG Text Support**: Custom character names and author credits

## File Structure

### Core Files
- `CustomCharacters.hpp` - Main header file with all declarations
- `CustomCharacterState.cpp` - State management and skin selection logic
- `CustomCharacterRaceAndNetwork.cpp` - Race and online multiplayer integration
- `CustomCharacterMenu.cpp` - Character select menu modifications
- `CustomCharacterAssets.cpp` - Asset loading (BRRES, TPL, etc.)
- `CustomCharacterLooseVoices.cpp` - Loose voice file system
- `CustomCharacterSoundEffects.cpp` - Sound effect overrides
- `LoadCustomAnimations.cpp` - Custom animation system

## How It Works

### Skin Naming Convention
Custom character skins use the format: `{character}-{table}.brres`

Examples:
- `mr-1.brres` - Mario skin #1
- `mr-2.brres` - Mario skin #2
- `pc-1.brres` - Peach skin #1
- `lg-1.brres` - Luigi skin #1

### File Locations
Place custom character files in:
- `/Scene/Model/Driver/{character}-{table}.brres` - Driver models
- `/sound/{fileId}.{character}-{table}.{extension}` - Sound effects
- Voice files (see CustomCharacterLooseVoices.cpp for details)

### Skin Selection
- In character select, use **L/R buttons** to cycle through available skins
- Each character can have up to 50 custom skins (table 1-50)
- Table 0 is always the default/vanilla character

### Online Play
- Custom skins are visible to other players who have the same custom character files
- The system sends skin table IDs in network packets
- Setting: SETTING_DISPLAYCUSTOMSKINS in Misc settings controls visibility

### Settings Integration

Added to `SettingsParam.hpp`:
```cpp
enum MiscSettings {
    // ...
    SETTING_LOOSEARCHIVEOVERRIDES = 1 + 8,
    SETTING_DISPLAYCUSTOMSKINS = 2 + 8,
};

enum DisplayCustomSkinsToggle {
    DISPLAYCUSTOMSKINS_ENABLED = 0x0,
    DISPLAYCUSTOMSKINS_DISABLED = 0x1,
};
```

### BMG Text IDs

Custom character names and authors use BMG IDs:
- Names: `0x6a00 + (character << 16) + table`
- Authors: `0x7a00 + (character << 16) + table`

Example for Mario skin #1:
- Name BMG: `0x00006a01`
- Author BMG: `0x00007a01`

## Custom Animations

Supported optional CHR animations:
- `shockHit` - Replaces shock damage animation
- `starUse` - Plays during Star power
- `megaUse` - Plays during Mega Mushroom
- `waitBeforeStart` - Plays before race countdown
- `shockDodgeStar` - Plays when dodging shock with Star

Each CHR animation can have an optional matching PAT0 texture animation.

## Integration with SillyKartWii

### Modified Files
1. `CharacterLayers.cpp` - Added comment pointing to CustomCharacters
2. `UI/UI.hpp` - Added BMG IDs for custom characters
3. `Settings/SettingsParam.hpp` - Added DISPLAYCUSTOMSKINS setting

### Required Dependencies
- IO/SDIO.hpp (already present)
- All standard Pulsar includes
- MKW decomp headers

## Building

The custom character system will compile with your regular build process. Make sure all `.cpp` files in this folder are included in your build system (Makefile, project files, etc.).

## Credits

Original implementation by the Retro Rewind team.
Ported to SillyKartWii by [Your Name].

## Notes

- The system uses memory-mapped character name arrays to swap between vanilla and custom postfixes
- Raw BRRES files are cached in heaps tied to the current GameScene
- Character select author text is displayed below character names (when not in multiplayer)
- Offline CPU racers get stable random skins based on race signature
- Ghost data can save and restore the skin table used during recording
