/*
 * Dolphin Emulator IOS Device Interface
 * 
 * This file provides an interface to communicate with Dolphin Emulator's
 * custom IOS device (/dev/dolphin) for features like Discord Rich Presence.
 * 
 * Based on: https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/IOS/DolphinDevice.cpp
 * 
 * License: Public Domain / MIT
 */

#ifndef __DOLPHINIOS_HPP
#define __DOLPHINIOS_HPP

#include <kamek.hpp>
#include <core/rvl/ipc/ipc.hpp>

namespace Dolphin {

enum DolphinIOCTL {
    IOCTL_DOLPHIN_GET_ELAPSED_TIME = 0x01,
    IOCTL_DOLPHIN_GET_VERSION = 0x02,
    IOCTL_DOLPHIN_GET_SPEED_LIMIT = 0x03,
    IOCTL_DOLPHIN_SET_SPEED_LIMIT = 0x04,
    IOCTL_DOLPHIN_GET_CPU_SPEED = 0x05,
    IOCTL_DOLPHIN_GET_REAL_PRODUCTCODE = 0x06,
    IOCTL_DOLPHIN_DISCORD_SET_CLIENT = 0x07,
    IOCTL_DOLPHIN_DISCORD_SET_PRESENCE = 0x08,
    IOCTL_DOLPHIN_DISCORD_RESET = 0x09,
    IOCTL_DOLPHIN_GET_SYSTEM_TIME = 0x0A,
};

bool IsEmulator();

bool SetDiscordClient(char* clientID);

bool SetDiscordPresence(
    char* details,
    char* state,
    char* largeImageKey,
    char* largeImageText,
    char* smallImageKey,
    char* smallImageText,
    u64 startTimestamp,
    u64 endTimestamp,
    u32 partySize,
    u32 partyMax);

bool ResetDiscord();

bool GetSystemTime(u64& systemTime);

}  // namespace Dolphin

#endif
