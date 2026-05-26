#include <kamek.hpp>

namespace Pulsar {
namespace Codes {

// Show Position in Live View [MrBean35000vr]
kmWrite32(0x806335B0, 0x3860021A);

// Disable Camera Shaking from Bombs [ZPL]
kmWrite32(0x805a906c, 0x4E800020);

// instant Voting Roulette Decide [Ro]
kmWrite32(0x80643BC4, 0x60000000);
kmWrite32(0x80643C2C, 0x60000000);

// Allow Pausing Before Race Starts [Sponge]
kmWrite32(0x80856a28, 0x40810050);

// No Disconnect on Countdown [_tZ]
kmWrite32(0x80655578, 0x60000000);

// Allow WFC on Wiimmfi Patched ISOs
kmWrite32(0x800EE3A0, 0x2C030000);
kmWrite32(0x800ECAAC, 0x7C7E1B78);

// Cancel Friend Room Joining by Pressing B [Ro]
extern "C" void ptr_inputBase(void*);
asmFunc friendRoomJoinCancel() {
    ASM(
        nofralloc;
        lis r31, ptr_inputBase @ha;
        lwz r31, ptr_inputBase @l(r31);
        lhz r31, 0x60(r31);
        andi.r31, r31, 0x2;
        beq end;
        li r3, 3;
        end:;
        cmpwi r3, 3;
        blr;
    )
}
kmCall(0x805DD85C, friendRoomJoinCancel);

}
}
