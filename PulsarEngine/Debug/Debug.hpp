#ifndef _PUL_Debug_
#define _PUL_Debug_

#include <core/rvl/os/OSError.hpp>

namespace Pulsar {
namespace Debug {

void FatalError(const char* string);
void LaunchSoftware();
struct GPR {
    void Set(const OS::Context& context, u32 idx, u32 regValue) {
        gpr = context.gpr[idx];
        name = 'r00:' + regValue;
    }
    u32 name;
    u32 gpr;
};

struct FPR {
    void Set(const OS::Context& context, u32 idx, u32 regValue) {
        fpr = context.fpr[idx];
        name = 'f00:' + regValue;
    }
    u32 name;
    double fpr;
};

struct StackFrame {
    StackFrame() : spName('sp: '), sp(0), lrName('lr: '), lr(0) {};
    u32 spName;
    u32 sp;
    u32 lrName;
    u32 lr;
};

struct ExceptionFile {
    explicit ExceptionFile(const OS::Context& context);

    u32 magic;
    u32 region;
    u32 reserved;
    OS::Error error;
    GPR srr0;
    GPR srr1;
    GPR msr;
    GPR cr;
    GPR lr;
    GPR gprs[32];
    FPR fprs[32];
    FPR fpscr;
    StackFrame frames[10];
    
    // SillyKartWii specific crash data
    u8 vanillaMode;              // Game mode
    u8 sillyMode;                // Silly mode setting
    u8 sectionMode;              // Current section
    u8 currentPage;              // Current UI page
    u16 lastPlayedTrack;         // Last track played
    u16 sillyEffects;            // Silly effects bitmap
    u16 sillySounds;             // Silly sounds bitmap
    u8 sillyPhysics;             // Silly physics setting
    u8 braikeDrifting;           // Brake drifting setting
    u16 freeRoamMode;            // Free roam setting
    u8 padding[1];               // Padding for alignment
};

}//namespace Debug
}//namespace Pulsar

#endif