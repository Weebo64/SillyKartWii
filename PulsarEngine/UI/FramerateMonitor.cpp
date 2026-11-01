#include <kamek.hpp>
#include <PulsarSystem.hpp>

float CurrentFramerate = 0.0f;

asmFunc GetFramerate() {
    ASM(
    nofralloc;
    stfs      f0, 0x24(r31);
    lis       r8, CurrentFramerate@h;
    ori       r8, r8, CurrentFramerate@l;
    stfs      f0, 0x0(r8);
    li        r8, 0;
    blr;
    )
}
kmCall(0x8021a0d8, GetFramerate);