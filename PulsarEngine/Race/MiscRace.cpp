#include <kamek.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/UI/Ctrl/CtrlRace/CtrlRaceBalloon.hpp>
#include <MarioKartWii/UI/Ctrl/CtrlRace/CtrlRaceResult.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/Driver/DriverManager.hpp>
#include <Settings/Settings.hpp>

namespace Pulsar {
namespace Race {

static void NonGhostPlayerCount(RacedataScenario& scenario, u8* playerCount, u8* screenCount, u8* localPlayerCount) {
    scenario.ComputePlayerCounts(playerCount, screenCount, localPlayerCount);
    System* system = System::sInstance;
    u8 realPlayers = *playerCount;
    if (scenario.settings.gamemode != MODE_TIME_TRIAL) for (int i = 0; i < 12; ++i) if (scenario.players[i].playerType == PLAYER_GHOST) --realPlayers;
    system->nonTTGhostPlayersCount = realPlayers;
}
kmCall(0x8052fc78, NonGhostPlayerCount);

kmWrite32(0x807997e0, 0x60000000);
//Starting item for OTT and TT, id is TRIPLE_MUSHROOM by default
static void SetStartingItem(Item::PlayerInventory& inventory, ItemId id, bool isItemForcedDueToCapacity) {
    register u32 playerId;
    asm(mr playerId, r29;);
    if (Racedata::sInstance->racesScenario.players[playerId].playerType == PLAYER_CPU) return;
    const System* system = System::sInstance;
    const bool isTT = DriverMgr::isTT;
    if (isTT || system->IsContext(PULSAR_MODE_OTT)) {
        bool isFeather;
        if (isTT) {
            const TTMode mode = system->ttMode;
            isFeather = (mode == TTMODE_150_FEATHER || mode == TTMODE_200_FEATHER);
        }
        else isFeather = system->IsContext(PULSAR_FEATHER);
        if (isFeather) id = BLOOPER;
        inventory.SetItem(id, isItemForcedDueToCapacity);
        if (isFeather) inventory.currentItemCount = 3;
    }
}
kmCall(0x80799808, SetStartingItem);

//From JoshuaMK, ported to C++ by Brawlbox and adapted as a setting
static int MiiHeads(Racedata* racedata, u32 unused, u32 unused2, u8 id) {
    CharacterId charId = racedata->racesScenario.players[id].characterId;
    bool miiHeadFroom = HOSTSETTING_ALLOW_MIIHEADS_ENABLED;
    if (RKNet::Controller::sInstance->roomType == RKNet::ROOMTYPE_FROOM_HOST || RKNet::Controller::sInstance->roomType == RKNet::ROOMTYPE_FROOM_NONHOST) {
        miiHeadFroom = System::sInstance->IsContext(PULSAR_MIIHEADS) ? HOSTSETTING_ALLOW_MIIHEADS_ENABLED : HOSTSETTING_ALLOW_MIIHEADS_DISABLED;
    }
    if (Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_MII) == RACESETTING_MII_ENABLED) {
        if (miiHeadFroom == HOSTSETTING_ALLOW_MIIHEADS_ENABLED) {
            if (charId < MII_M) {
                if (id == 0) charId = MII_M;
                else if (RKNet::Controller::sInstance->connectionState != 0) charId = MII_M;
            }
        }
    }
    return charId;
}
kmCall(0x807eb154, MiiHeads);
kmWrite32(0x807eb15c, 0x60000000);
kmWrite32(0x807eb160, 0x88de01b4);

//credit to XeR for finding the float address
static void BattleGlitchEnable() {
    const u8 val = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_BATTLE);
    float maxDistance = 7500.0f;
    if (val == RACESETTING_BATTLE_GLITCH_ENABLED) maxDistance = 75000.0f;
    System* system = System::sInstance;
    if (system->IsContext(PULSAR_MODE_OTT)) {
        const Input::RealControllerHolder* controllerHolder = SectionMgr::sInstance->pad.padInfos[0].controllerHolder;
        const ControllerType controllerType = controllerHolder->curController->GetType();
        const u16 inputs = controllerHolder->inputStates[0].buttonRaw;
        const u16 newInputs = (inputs & ~controllerHolder->inputStates[1].buttonRaw);
        u32 toggleInput = PAD::PAD_BUTTON_Y;
        switch (controllerType) {
        case NUNCHUCK:
            toggleInput = WPAD::WPAD_BUTTON_DOWN;
            break;
        case WHEEL:
            toggleInput = WPAD::WPAD_BUTTON_MINUS;
            break;
        case CLASSIC:
            toggleInput = WPAD::WPAD_CL_TRIGGER_ZL;
            break;
        }
        if ((newInputs & toggleInput) == toggleInput) system->ottHideNames = !system->ottHideNames;
        if (system->ottHideNames) maxDistance -= maxDistance;
    }
    RaceBalloons::maxDistanceNames = maxDistance;
}
RaceFrameHook BattleGlitch(BattleGlitchEnable);

kmWrite32(0x8085C914, 0x38000000); //times at the end of races in VS
static void DisplayTimesInsteadOfNames(CtrlRaceResult& result, u8 id) {
    result.FillFinishTime(id);
}
kmCall(0x8085d460, DisplayTimesInsteadOfNames); //for WWs

//don't hide position tracker (MrBean35000vr)
kmWrite32(0x807F4DB8, 0x38000001);

//Draggable blue shells
static void DraggableBlueShells(Item::PlayerObj& sub) {
    if (Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_BLUES) == RACESETTING_DRAGGABLE_BLUES_DISABLED) {
        sub.isNotDragged = true;
    }
}
kmBranch(0x807ae8ac, DraggableBlueShells);

//Coloured Minimap
kmWrite32(0x807DFC24, 0x60000000);

//No Team Invincibility
kmWrite32(0x8056fd24, 0x38000000); //KartCollision::CheckKartCollision()
kmWrite32(0x80572618, 0x38000000); //KartCollision::CheckItemCollision()
kmWrite32(0x80573290, 0x38000000); //KartCollision::HandleFIBCollision()
kmWrite32(0x8068e2d0, 0x38000000); //PlayerEffects ctor
kmWrite32(0x8068e314, 0x38000000); //PlayerEffects ctor
kmWrite32(0x807a7f6c, 0x38c00000); //FIB are always red
kmWrite32(0x807b0bd4, 0x38000000); //pass TC to teammate
kmWrite32(0x807bd2bc, 0x38000000); //RaceGlobals
kmWrite32(0x807f18c8, 0x38000000); //TC alert

//Accurate Explosion Damage (MrBean, CLF)
kmWrite16(0x80572690, 0x4800);
kmWrite16(0x80569F68, 0x4800);


//CtrlItemWindow
kmWrite24(0x808A9C16, 'PUL'); //item_window_new -> item_window_PUL

const char* ChangeItemWindowPane(ItemId id, u32 itemCount) {
    const bool feather = System::sInstance->IsContext(PULSAR_FEATHER);
    const bool megaTC = System::sInstance->IsContext(PULSAR_MEGATC);
    const char* paneName;
    if (id == BLOOPER && feather) {
        if (itemCount == 2) paneName = "feather_2";
        else if (itemCount == 3) paneName = "feather_3";
        else paneName = "feather";
    }
    else if (id == THUNDER_CLOUD && megaTC && RKNet::Controller::sInstance->roomType != RKNet::ROOMTYPE_VS_WW) paneName = "megaTC";
    else paneName = GetItemIconPaneName(id, itemCount);
    return paneName;
}
kmCall(0x807f3648, ChangeItemWindowPane);
kmCall(0x807ef168, ChangeItemWindowPane);
kmCall(0x807ef3e0, ChangeItemWindowPane);
kmCall(0x807ef444, ChangeItemWindowPane);

kmWrite24(0x808A9FF3, 'PUL');

//Enhanced Pause Menu (Ro/Insane Kart Wii)
//replaces all pause menus with TT pause so you can restart in GP/VS/Battle
kmWrite32(0x8062c658, 0x38800019);
kmWrite32(0x8062c79c, 0x38800019);
kmWrite32(0x80633a98, 0x38600019);
kmWrite32(0x8062c8e0, 0x38800019);
kmWrite32(0x80633970, 0x38600019);
kmWrite32(0x8083d618, 0x60000000);

extern "C" void sInstance__8Racedata(void*);
extern "C" void sInstance__10SectionMgr(void*);
asmFunc EnhancedPauseMenu1() {
    ASM(
    lwz r3, sInstance__10SectionMgr@l (r6);
    lwz r12, sInstance__8Racedata@l (r6);
    lwz r0, 0x1760 (r12);
    cmpwi r0, 2;
    beq end;

    cmpwi r4, 0x49;
    beq decreaseRaceNum;

    cmpwi r4, 0x4A;
    bne end;

    li r4, 0x4B;

    cmpwi r0, 1;
    beq decreaseRaceNum;

    li r4, 0x4C;

    decreaseRaceNum:
    lwz r6, 0x98 (r3);
    lwz r31, 0x60 (r6);
    subi r31, r31, 1;
    stw r31, 0x60 (r6);

    li r31, 5;
    stw r31, 0x1764 (r12);

    end:
    mr r31, r5;
    blr;
    )
}
kmCall(0x806024d8, EnhancedPauseMenu1);

asmFunc EnhancedPauseMenu2() {
    ASM(
    lis r3, sInstance__8Racedata@ha;
    lwz r3, sInstance__8Racedata@l (r3);
    lwz r4, 0x1760 (r3);
    cmpwi r4, 2;
    beq end;
    li r4, 0x1;
    stw r4, 0xD18 (r3);
    stw r4, 0xE08 (r3);
    stw r4, 0xEF8 (r3);

    end:
    li        r3, 0x6C4;
    blr;
    )
}
kmCall(0x80623df4, EnhancedPauseMenu2);

kmWrite32(0x80859068, 0x48000808);
kmWrite32(0x80858e38, 0x48000040);

}//namespace Race
}//namespace Pulsar