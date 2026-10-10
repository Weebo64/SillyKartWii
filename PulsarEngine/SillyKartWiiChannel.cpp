#include <PulsarSystem.hpp>
#include "Debug/Debug.hpp"
#include "SillyKartWiiChannel.hpp"
#include "IO/SDIO.hpp"
#include <Dolphin/DolphinIOS.hpp>

namespace Pulsar {

bool IsNewChannel() {
    return *reinterpret_cast<u32 *>(SKW_SIGNATURE_ADDRESS) == SKW_SIGNATURE;
}

bool NewChannel_UseSeparateSavegame() {
    return (*reinterpret_cast<u8 *>(SKW_BITFLAGS_ADDRESS) & SKW_BITFLAG_SEPARATE_SAVEGAME) == SKW_BITFLAG_SEPARATE_SAVEGAME;
}

void NewChannel_WriteLoadedFromRREphFile() {
    if (IO::sInstance == nullptr) return;
    IO::sInstance->CreateAndOpen(SKW_LOADED_FROM_CHANNEL_EPH_FILE_PATH, IOS::MODE_READ_WRITE);
    IO::sInstance->Close();
    // Check the file was actually written
    // Also for some reason without this it doesnt get wrtten. I guess there's some flush/sync issues
    if (!IO::sInstance->OpenFile(SKW_LOADED_FROM_CHANNEL_EPH_FILE_PATH, IOS::MODE_READ)) {
        Debug::FatalError(SKW_LOADED_FROM_CHANNEL_EPH_FILE_PATH " was not written properly.");
    }
}

void NewChannel_WriteCrashEphFile() {
    if (IO::sInstance == nullptr) return;
    IO::sInstance->CreateAndOpen(SKW_CRASH_EPH_FILE_PATH, IOS::MODE_READ_WRITE);
    IO::sInstance->Close();
}

void NewChannel_Init() {
    u32 channelVersion = *reinterpret_cast<u32 *>(SKW_ABI_VERSION_ADDRESS);
    u32 requiredVersion = SKW_ABI_VERSION;

    // Make sure the channel is compatible with this Code.pul
    if (channelVersion != requiredVersion) {
        char message[256];
        snprintf(message, sizeof(message),
                 "This version of Silly Kart Wii is incompatible with the version of the channel (abi%d != abi%d).\n"
                 "You can usually fix this by updating both SKW and the channel to the latest version.",
                 channelVersion, requiredVersion);
        Debug::FatalError(message);
    }
}

}  // namespace Pulsar
