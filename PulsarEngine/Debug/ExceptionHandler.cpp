#include <kamek.hpp>
#include <core/System/SystemManager.hpp>
#include <core/rvl/base/ppc.hpp>
#include <core/rvl/OS/OSthread.hpp>
#include <core/rvl/OS/OSTitle.hpp>
#include <core/rvl/OS/OSBootInfo.hpp>
#include <core/rvl/PAD.hpp>
#include <core/rvl/KPAD.hpp>
#include <core/egg/Exception.hpp>
#include <Debug/Debug.hpp>
#include <PulsarSystem.hpp>
#include <IO/IO.hpp>
#include <SillyKartWiiChannel.hpp>
#include <Settings/Settings.hpp>
#include <SillyKartWii.hpp>
#include <SlotExpansion/CupsConfig.hpp>
#include <RuntimeWrite.hpp>

namespace Pulsar {
namespace Debug {

OS::Thread* crashThread = nullptr;
static u16 crashError = 0;
static u32 crashCounter = 0;

using namespace nw4r;

void FatalError(const char* string) {
    GX::Color fg;
    fg.rgba = 0xFFFFFFFF;
    GX::Color bg = {0};
    OS::Fatal(fg, bg, string);
}

#pragma suppress_warnings on
void LaunchSoftware() {
    s32 result = IO::OpenFix("/title/00010001/534b5743/content/title.tmd\0", IOS::MODE_NONE);  // Launcher
    if (result >= 0) {
        ISFS::Close(result);
        OS::__LaunchTitle(0x00010001, 0x534b5743);
        return;
    }
    OS::__LaunchTitle(0x1, 0x2);  // Launch Wii Menu if channel isn't found
}
#pragma suppress_warnings reset

kmCall(0x8000b188, LaunchSoftware);

// Credit Star

//Data Shown
//kmWrite16(0x802A7410, 0x00000023);
//Show Handler
//kmWrite32(0x802A7404, 0x00000000);
//Show StackTrace
kmWrite32(0x80023948, 0x281e0007);
//Max number of lines
kmWrite32(0x80009324, 0x38800068);

//Lines on the screen and x-pos
static void SetConsoleParams() {
    db::detail::ConsoleHead* console = EGG::Exception::console;
    console->viewLines = 0x15;
    console->viewPosX = 0x5;
    console->viewPosY = 0xA;
}
BootHook ConsoleParams(SetConsoleParams, 1);

ExceptionFile::ExceptionFile(const OS::Context& context) : magic('PULD'), region(*reinterpret_cast<u32*>(OS::BootInfo::mInstance.diskID.gameName)), reserved(-1) {
    this->srr0.name = 'srr0';
    this->srr0.gpr = context.srr0;
    this->srr1.name = 'srr1';
    this->srr1.gpr = context.srr1;
    this->msr.name = 'msr:';
    this->msr.gpr = PPCMfmsr();
    this->cr.name = 'cr: ';
    this->cr.gpr = context.cr;
    this->lr.name = 'lr: ';
    this->lr.gpr = context.lr;
    for (u32 i = 0; i < 32; ++i) {
        this->gprs[i].gpr = context.gpr[i];  // r00: //r01:
        this->fprs[i].fpr = context.fpr[i];
        const u32 tens = i / 10U;
        const u32 units = i - tens * 10U;
        const u32 regValue = units * 0x100U + tens * 0x10000U;
        this->gprs[i].name = 'r00:' + regValue;
        this->fprs[i].name = 'f00:' + regValue;
    }
    this->fpscr.name = 'fscr';
    this->fpscr.fpr = context.fpscr;
    u32* sp = (u32*)context.gpr[1];
    for (int i = 0; i < 10; ++i) {
        if (sp == nullptr || (u32)sp == 0xFFFFFFFF) break;
        this->frames[i].sp = (u32)sp;
        this->frames[i].lr = sp[1];
        sp = (u32*)*sp;
    }
}

u8 GameMode = 0;
u8 MyStuff = 0;
u8 SectionMode = 0;
u16 LastPlayedTrack = 0;


#define GAMEPLAY2_BIT (1 << 0)
#define GAMEPLAY4_BIT (1 << 1)
#define GAMEPLAY5_BIT (1 << 2)
#define GAMEPLAY6_BIT (1 << 3)
#define GAMEPLAY8_BIT (1 << 4)
#define GAMEPLAY9_BIT (1 << 5)
#define GAMEPLAYA_BIT (1 << 6)
#define GAMEPLAYD_BIT (1 << 7)
#define GAMEPLAYE_BIT (1 << 8)
#define GAMEPLAYF_BIT (1 << 9)
#define GAMEPLAYG_BIT (1 << 10)
#define GAMEPLAY1_BIT (1 << 11)
#define GAMEPLAY3_BIT (1 << 12)
#define GAMEPLAY7_BIT (1 << 13)
#define GAMEPLAYB_BIT (1 << 14)
#define U16_FREE_ROAM_BIT (1 << 15)

u16 GameplayBitmap = 0;

#define SET_GAMEPLAY_BIT(bitmap, bit) ((bitmap) |= (bit))
#define CLEAR_GAMEPLAY_BIT(bitmap, bit) ((bitmap) &= ~(bit))
#define CHECK_GAMEPLAY_BIT(bitmap, bit) (((bitmap) & (bit)) != 0)

void UpdateExceptionHandlerBits() {
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    PulsarId pulsarId = cupsConfig->GetWinning();
    GameMode = Racedata::sInstance->menusScenario.settings.gamemode;
    MyStuff = 0; // Not available in SillyKartWii
    SectionMode = SectionMgr::sInstance->curSection->sectionId;
    LastPlayedTrack = CupsConfig::ConvertTrack_PulsarIdToRealId(pulsarId);
    GameplayBitmap = 0;
    if (U16_GAMEPLAY2) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY2_BIT);
    if (U16_GAMEPLAY4) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY4_BIT);
    if (U16_GAMEPLAY5) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY5_BIT);
    if (U16_GAMEPLAY6) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY6_BIT);
    if (U16_GAMEPLAY8) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY8_BIT);
    if (U16_GAMEPLAY9) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY9_BIT);
    if (U16_GAMEPLAYA) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYA_BIT);
    if (U16_GAMEPLAYD) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYD_BIT);
    if (U16_GAMEPLAYE) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYE_BIT);
    if (U16_GAMEPLAYF) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYF_BIT);
    if (U16_GAMEPLAYG) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYG_BIT);
    if (U16_GAMEPLAY1) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY1_BIT);
    if (U16_GAMEPLAY3) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY3_BIT);
    if (U16_GAMEPLAY7) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAY7_BIT);
    if (U16_GAMEPLAYB) SET_GAMEPLAY_BIT(GameplayBitmap, GAMEPLAYB_BIT);
    if (U16_FREE_ROAM) SET_GAMEPLAY_BIT(GameplayBitmap, U16_FREE_ROAM_BIT);
}
static PageLoadHook2 UPDATE_EXCEPTION_HANDLER(UpdateExceptionHandlerBits);

static void WriteHeaderCrash(u16 error, const OS::Context* context, u32 dsisr, u32 dar) {
    crashError = error;
    crashThread = const_cast<OS::Thread*>(reinterpret_cast<const OS::Thread*>(context));
    db::ExceptionHead& exception = db::ExceptionHead::mInstance;
    exception.displayedInfo = 0x23;
    exception.callbackArgs = nullptr;

    db::Exception_Printf_("Oops. SKWii just crashed.\nIf this is the first time, try to replicate it.\nIf it happened multiple times, report this\ncrash with the Crash.pul to\nWeebo's Discord Server:\n\nhttps://discord.gg/j3MeZAkYPn\n\nPress A to exit and save the Crash.pul.\nWe are sorry for the inconvenience.\n\nFurther Crash Info:\n\n");
    db::Exception_Printf_("Version: v2.0\n");
    db::Exception_Printf_("Game Mode: %d\n", GameMode);
    db::Exception_Printf_("My Stuff: %d\n", MyStuff);
    db::Exception_Printf_("Section Mode: 0x%X\n", SectionMode);
    db::Exception_Printf_("Page: 0x%X\n", U8_CURRENT_PAGE);
    if(REGION == 'P') db::Exception_Printf_("Region: PAL\n");
    else if(REGION == 'E') db::Exception_Printf_("Region: NTSC-U\n");
    else if(REGION == 'J') db::Exception_Printf_("Region: NTSC-J\n");
    else if(REGION == 'K') db::Exception_Printf_("Region: NTSC-K\n");
    db::Exception_Printf_("Last Played Track: %d\n", LastPlayedTrack);
    db::Exception_Printf_("Game Mode Bitmap: 0x%X\n\n", GameplayBitmap);
    db::PrintContext_(error, context, dsisr, dar);
}
kmCall(0x80023484, WriteHeaderCrash);


static void CreateCrashFile(s32 channel, KPAD::Status buff[], u32 count) {
    IO* io = IO::sInstance;
    
    // Save crash file first, before checking for button press
    if (crashThread != nullptr && crashCounter == 0) {
        crashCounter++;  // Ensure we only save once
        
        OS::Thread* thread = crashThread;
        alignas(0x20) ExceptionFile exception(thread->context);
        exception.error = static_cast<OS::Error>(crashError);
        exception.vanillaMode = GameMode;
        exception.sillyMode = U8_SILLY_MODE;
        exception.sectionMode = SectionMode;
        exception.currentPage = U8_CURRENT_PAGE;
        exception.lastPlayedTrack = LastPlayedTrack;
        exception.sillyEffects = U16_SILLY_EFFECTS;
        exception.sillySounds = U16_SILLY_SOUNDS;
        exception.sillyPhysics = U8_SILLY_PHYSICS;
        exception.braikeDrifting = U8_BRAKEDRIFTING;
        exception.freeRoamMode = U16_FREE_ROAM;
        
        char path[IOS::ipcMaxPath];
        const System* system = System::sInstance;
        bool savedSuccessfully = false;
        
        // Try to save to mod folder first
        if (system != nullptr && io != nullptr) {
            snprintf(path, IOS::ipcMaxPath, "%s/Crash.pul", system->GetModFolder());
            if (io->CreateAndOpen(path, IOS::MODE_READ_WRITE)) {
                io->Overwrite(sizeof(ExceptionFile), &exception);
                io->Close();
                savedSuccessfully = true;
            }
        }
        
        // If mod folder failed, try NAND
        if (!savedSuccessfully) {
            // Create NAND directories
            ISFS::CreateDir("/shared2", 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            ISFS::CreateDir("/shared2/SKWii_Crash", 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            
            snprintf(path, IOS::ipcMaxPath, "/shared2/SKWii_Crash/Crash.pul");
            
            // Create file with ISFS
            s32 result = ISFS::CreateFile(path, 0, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE, IOS::MODE_READ_WRITE);
            if (result >= 0 || result == -105) { // -105 = already exists
                s32 fd = ISFS::Open(path, ISFS::MODE_READ_WRITE);
                if (fd >= 0) {
                    ISFS::Write(fd, &exception, sizeof(ExceptionFile));
                    ISFS::Close(fd);
                    savedSuccessfully = true;
                }
            }
        }
    }
    
    // Now check for button press to exit
    bool exit = false;
    if (io == nullptr) {
        exit = true;
    } else {
        KPAD::Read(channel, buff, count);
        u8 wiimoteExtension = buff[0].extension;
        if (buff[0].error == WPAD::WPAD_ERR_NONE && wiimoteExtension < WPAD::WPAD_DEV_FUTURE &&
            (wiimoteExtension == WPAD::WPAD_DEV_CLASSIC && buff[0].extStatus.cl.trig & WPAD::WPAD_CL_BUTTON_A || buff[0].trig & WPAD::WPAD_BUTTON_A))
            exit = true;
        else {
            PAD::Status padStatus[4];
            PAD::Read(&padStatus[0]);
            for (int channel = 0; channel < 4; ++channel) {
                if (padStatus[channel].error == PAD::PAD_ERR_NONE) {
                    if (padStatus[channel].buttons & PAD::PAD_BUTTON_A) {
                        exit = true;
                        break;
                    }
                }
            }
        }
    }
    
    if (exit) {
        if (crashThread != nullptr) {
            OS::DetachThread(crashThread);
            OS::CancelThread(crashThread);
        }
        LaunchSoftware();
    }
}
kmCall(0x80226610, CreateCrashFile);

}  // namespace Debug
}  // namespace Pulsar
