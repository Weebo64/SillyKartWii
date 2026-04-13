/*
 * Discord Rich Presence Integration for Mario Kart Wii Mods
 * 
 * This file provides Discord Rich Presence support for Pulsar-based
 * Mario Kart Wii mods using Dolphin Emulator's IOS device interface.
 * 
 * Usage:
 * 1. Replace "YOUR_CLIENT_ID_HERE" with your Discord Application ID
 * 2. Replace "YOUR_MOD_NAME" with your mod's name
 * 3. Replace "icon_big" with your Discord asset key
 * 4. Add to your project and compile
 * 
 * Requirements:
 * - Dolphin/DolphinIOS.hpp and .cpp files
 * - Pulsar framework
 * - Discord Application with uploaded assets
 * 
 * License: Public Domain / MIT (choose your preference)
 */

#include <Discord/Discord.hpp>
#include <Dolphin/DolphinIOS.hpp>
#include <MarioKartWii/Race/RaceInfo/RaceInfo.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/Scene/GameScene.hpp>
#include <MarioKartWii/UI/Section/Section.hpp>
#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <UI/UI.hpp>
#include <SlotExpansion/UI/ExpansionUIMisc.hpp>

namespace Discord {

static bool hasWrittenClientID = false;
static int frameCount = 0;
static u64 startTimeStamp = 0;
static SectionId prevSectionId = SECTION_NONE;

void CleanBMGMessage(wchar_t* dest, const wchar_t* src) {
    int inc = 0;
    for (int i = 0; i < 0x100 && src[i]; i++) {
        if (src[i] == 0x001a) {
            u8 size = *(u8*)(&src[i + 1]);
            i += (size / 2) - 1;
        } else {
            dest[inc] = src[i];
            inc++;
        }
    }
}

void DiscordRichPresence(Section* _this) {
    _this->Update();
    
    if (!Dolphin::IsEmulator()) {
        return;
    }

    if (_this->sectionId == prevSectionId && ((frameCount++ % 900) != 0)) {
        return;
    }

    if (!hasWrittenClientID) {
        Dolphin::SetDiscordClient("YOUR_CLIENT_ID_HERE");
        hasWrittenClientID = true;
    }

    char* state = "";
    char* details = "In a Menu";
    char* largeImageText = "YOUR_MOD_NAME";
    int minPlayers = 0;
    int maxPlayers = 0;

    if (_this->sectionId != prevSectionId) {
        Dolphin::GetSystemTime(startTimeStamp);
        prevSectionId = _this->sectionId;
    }

    wchar_t trackNameW[0x100];
    char trackName[0x100];
    memset(trackNameW, 0, 0x100);

    u32 bmgId = Pulsar::UI::GetCurTrackBMG();
    const wchar_t* msg = Pulsar::UI::GetCustomMsg(bmgId);
    if (msg && Raceinfo::sInstance && Raceinfo::sInstance->IsAtLeastStage(RACESTAGE_INTRO)) {
        CleanBMGMessage(trackNameW, msg);
        wcstombs(trackName, trackNameW, 0x100);
        state = trackName;
    }

    RKNet::Controller* controller = RKNet::Controller::sInstance;
    if (controller) {
        RKNet::ControllerSub& sub = controller->subs[controller->currentSub];
        maxPlayers = 12;
        minPlayers = sub.playerCount;
    }

    switch (_this->sectionId) {
        case SECTION_GP:
            details = "Grand Prix";
            break;
        case SECTION_TT:
            details = "Time Trials";
            break;
        case SECTION_P1VS:
        case SECTION_P2VS:
        case SECTION_P3VS:
        case SECTION_P4VS:
            details = "VS Race";
            break;
        case SECTION_P1TEAM_VS:
        case SECTION_P2TEAM_VS:
        case SECTION_P3TEAM_VS:
        case SECTION_P4TEAM_VS:
            details = "Team VS";
            break;
        case SECTION_P1BATTLE:
        case SECTION_P2BATTLE:
        case SECTION_P3BATTLE:
        case SECTION_P4BATTLE:
            details = "Battle";
            break;
        case SECTION_MISSION_MODE:
            details = "Mission Mode";
            break;
        case SECTION_TOURNAMENT:
            details = "Tournament";
            break;
        case SECTION_GP_REPLAY:
            details = "Watching GP Replay";
            break;
        case SECTION_TT_REPLAY:
        case SECTION_WATCH_GHOST_FROM_CHANNEL:
        case SECTION_WATCH_GHOST_FROM_DOWNLOADS:
        case SECTION_WATCH_GHOST_FROM_MENU:
            details = "Watching TT Replay";
            break;
        case SECTION_P1_WIFI:
        case SECTION_P1_WIFI_FROM_FROOM_RACE:
        case SECTION_P1_WIFI_FROM_FIND_FRIEND:
        case SECTION_P2_WIFI:
        case SECTION_P2_WIFI_FROM_FROOM_RACE:
        case SECTION_P2_WIFI_FROM_FIND_FRIEND:
            details = "WiFi Menu";
            break;
        case SECTION_P1_WIFI_VS_VOTING:
        case SECTION_P2_WIFI_VS_VOTING:
            details = "Voting (WiFi VS)";
            break;
        case SECTION_P1_WIFI_BATTLE_VOTING:
        case SECTION_P2_WIFI_BATTLE_VOTING:
            details = "Voting (WiFi Battle)";
            break;
        case SECTION_P1_WIFI_FROOM_VS_VOTING:
        case SECTION_P2_WIFI_FROOM_VS_VOTING:
            details = "Voting (Friend Room VS)";
            break;
        case SECTION_P1_WIFI_FROOM_TEAMVS_VOTING:
        case SECTION_P2_WIFI_FROOM_TEAMVS_VOTING:
            details = "Voting (Friend Room Team VS)";
            break;
        case SECTION_P1_WIFI_FROOM_BALLOON_VOTING:
        case SECTION_P2_WIFI_FROOM_BALLOON_VOTING:
            details = "Voting (Friend Room Balloon)";
            break;
        case SECTION_P1_WIFI_FROOM_COIN_VOTING:
        case SECTION_P2_WIFI_FROOM_COIN_VOTING:
            details = "Voting (Friend Room Coin)";
            break;
        case SECTION_P1_WIFI_VS:
        case SECTION_P2_WIFI_VS:
            details = "Racing (WiFi VS)";
            break;
        case SECTION_P1_WIFI_BT:
        case SECTION_P2_WIFI_BT:
            details = "Racing (WiFi Battle)";
            break;
        case SECTION_P1_WIFI_FRIEND_VS:
        case SECTION_P2_WIFI_FRIEND_VS:
            details = "Racing (Friend Room VS)";
            break;
        case SECTION_P1_WIFI_FRIEND_TEAMVS:
        case SECTION_P2_WIFI_FRIEND_TEAMVS:
            details = "Racing (Friend Room Team VS)";
            break;
        case SECTION_P1_WIFI_FRIEND_BALLOON:
        case SECTION_P2_WIFI_FRIEND_BALLOON:
            details = "Racing (Friend Room Balloon)";
            break;
        case SECTION_P1_WIFI_FRIEND_COIN:
        case SECTION_P2_WIFI_FRIEND_COIN:
            details = "Racing (Friend Room Coin)";
            break;
        case SECTION_P1_WIFI_VS_LIVEVIEW:
        case SECTION_P2_WIFI_VS_LIVEVIEW:
            details = "Spectating (WiFi VS)";
            break;
        case SECTION_P1_WIFI_BT_LIVEVIEW:
        case SECTION_P2_WIFI_BT_LIVEVIEW:
            details = "Spectating (WiFi Battle)";
            break;
        default:
            state = "";
            break;
    }

    Dolphin::SetDiscordPresence(
        details,
        state,
        "icon_big",
        largeImageText,
        "",
        "",
        startTimeStamp,
        0,
        minPlayers,
        maxPlayers);
}

kmCall(0x80635540, DiscordRichPresence);

}  // namespace Discord
