#include <kamek.hpp>

// Code by The Insane Kart Wii Team

asmFunc StarAnimation() {
    ASM(
  lhz       r0, 0xF6(r31);
  lwz       r12, 0x0(r31);
  lwz       r12, 0x4(r12);
  lwz       r12, 0x8(r12);
  andis.    r12, r12, 0x8000;
  beq-      loc_0x20;
  li        r0, 0x8;
  sth       r0, 0xF6(r31);

loc_0x20:
  blr;
    )
}

kmCall(0x807CD2DC, StarAnimation);
