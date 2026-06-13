// kart status made by Toadette Hack Fan/IKW Team

#include <kamek.hpp>
#include <SillyKartWii.hpp>

namespace Pulsar {
namespace UI {

kmWrite32(0x80001570, 0x40000000);
kmWrite32(0x80001578, 0x43A80000);

extern "C" void sInstance__10SectionMgr(void*);
asmFunc KartStatusesOnMinimap1() {
  ASM(
    nofralloc;
    lwz       r3, 0x1B8(r30);
    lis       r4, 0xC020;
    stw       r4, 0x614(r3);
    lis       r4, sInstance__10SectionMgr@ha;
    blr;
  )
}

extern "C" void sInstance__8Racedata(void*);
extern "C" void KartStatusEnd2(void*);
asmFunc KartStatusOnMinimap2() {
  ASM(
    nofralloc;
    stfs      f0, 0x48(r3);
    lis       r5, sInstance__8Racedata@ha;
    lbz       r12, 0x1B4(r28);
    lwz       r11, sInstance__8Racedata@l(r5);
    mulli     r12, r12, 0xF0;
    add       r11, r11, r12;
    lwz       r11, 0x38(r11);
    cmpwi     r11, 0;
    beq-      loc_0x38;
    cmpwi     r29, 0;
    mflr      r12;
    addi      r12, r12, 0xA4;
    mtlr      r12;
    beqlr-;    

  loc_0x38:
    b KartStatusEnd2;
  )
}

extern "C" void sInstance__Q24Kart7Manager(void*);
extern "C" void KartStatusEnd3(void*);
asmFunc KartStatusOnMinimap3() {
  ASM(
  nofralloc;
  lis       r12, sInstance__8Racedata@ha;
  lwz       r10, sInstance__8Racedata@l(r12);
  lwz       r12, sInstance__Q24Kart7Manager@l(r12);
  lwz       r12, 0x20(r12);
  lbz       r11, 0x1B4(r28);
  mulli     r27, r11, 0xF0;
  mulli     r11, r11, 0x4;
  add       r27, r10, r27;
  lwzx      r12, r12, r11;
  lwz       r6, 0x0(r12);
  bl        loc_0x40;
  subis     r26, r12, 0x3333;
  lis       r29, 0;
  subis     r10, r12, 0x3333;

loc_0x38:
  bdzf+     0, loc_0x38;
  lfs       f29, 0x0(r20);

loc_0x40:
  mflr      r8;
  lwz       r10, 0x1B8(r28);
  lwz       r29, 0x1BC(r28);
  lwz       r9, 0x1C0(r28);
  lwz       r4, 0x1C4(r28);
  lfs       f0, 0x44(r10);
  lfs       f2, 0x44(r9);
  lfs       f6, 0x48(r9);
  lfs       f7, 0x48(r10);
  lwz       r12, 0x28(r6);
  lfs       f3, 0x164(r12);
  lfs       f5, 0x168(r12);
  lfs       f4, 0x4(r8);
  fcmpo     cr0, f3, f4;
  ble-      loc_0x80;
  fmr       f3, f4;

loc_0x80:
  fcmpo     cr0, f5, f4;
  ble-      loc_0x8C;
  fmr       f5, f4;

loc_0x8C:
  lwz       r5, 0x4(r6);
  lwz       r7, 0xC(r5);
  andis.    r0, r7, 0xC;
  bne-      loc_0xE0;
  lfs       f4, 0x0(r8);
  fcmpo     cr0, f3, f4;
  bge-      loc_0xAC;
  fmr       f3, f4;

loc_0xAC:
  fcmpo     cr0, f5, f4;
  bge-      loc_0xE0;
  andis.    r0, r7, 0x1;
  bne-      loc_0xD4;
  lbz       r12, -0x24(r5);
  cmpwi     r12, 0;
  beq-      loc_0xDC;
  lhz       r12, -0x20(r5);
  cmpwi     r12, 0x4200;
  bge-      loc_0xDC;

loc_0xD4:
  lfs       f1, 0x8(r8);
  fadds     f4, f5, f1;

loc_0xDC:
  fmr       f5, f4;

loc_0xE0:
  andis.    r0, r7, 0x1000;
  li        r0, -0x1;
  beq-      loc_0xF0;
  li        r0, 0;

loc_0xF0:
  stw       r0, 0x149(r10);
  stb       r0, 0x14D(r10);
  lwz       r12, 0x58(r6);
  lbz       r12, 0x12(r12);
  cmpwi     r12, 0;
  bne-      loc_0x110;
  li        r0, 0x1;
  stb       r0, 0x80(r28);

loc_0x110:
  andis.    r0, r7, 0x800;
  li        r0, 0;
  beq-      loc_0x140;
  stb       r0, 0xB8(r9);
  stb       r0, 0xB8(r29);
  li        r0, 0x1;
  lfs       f3, 0x8(r30);
  fmr       f5, f3;
  lfs       f4, 0x40(r4);
  stfs      f4, 0x254(r10);
  fsubs     f4, f4, f4;
  stfs      f4, 0x40(r10);

loc_0x140:
  stb       r0, 0x2CF(r10);
  xori      r0, r0, 0x1;
  mulli     r12, r0, 0xFF;
  stb       r12, 0x14F(r10);
  lha       r12, 0xB8(r10);
  stb       r12, 0x2CC(r10);
  stb       r12, 0x4B4(r10);
  stb       r12, 0x69C(r10);
  fmuls     f2, f2, f3;
  fmuls     f0, f0, f3;
  fmuls     f6, f6, f5;
  fmuls     f7, f7, f5;
  stfs      f0, 0x44(r10);
  stfs      f2, 0x44(r29);
  stfs      f2, 0x44(r9);
  stfs      f2, 0x44(r4);
  stfs      f7, 0x48(r10);
  stfs      f7, 0x48(r29);
  stfs      f6, 0x48(r9);
  stfs      f6, 0x48(r4);
  lwz       r11, 0x98(r28);
  lwz       r11, 0x44(r11);
  lwz       r11, 0x0(r11);
  lwz       r11, 0xC(r11);
  li        r12, 0;
  stb       r12, 0x30(r11);
  stb       r12, 0xFC(r11);
  lwz       r12, 0x54(r6);
  lbz       r9, 0x14(r12);
  lbz       r7, 0x15(r12);
  lbz       r11, 0x16(r12);
  stb       r9, 0x149(r29);
  stb       r9, 0x34D(r29);
  stb       r7, 0x14B(r29);
  stb       r7, 0x34F(r29);
  stb       r11, 0x14D(r29);
  stb       r11, 0x351(r29);
  lwz       r11, 0x8(r5);
  andis.    r0, r11, 0x8000;
  li        r0, 0;
  li        r6, 0;
  beq-      loc_0x288;
  lbz       r11, 0xBB(r4);
  cmpwi     r11, 0;
  bne-      loc_0x224;
  lfs       f0, 0x18(r12);
  lfs       f1, 0x88(r30);
  fmuls     f0, f0, f1;
  fctiwz    f0, f0;
  stfd      f0, -0x8(r1);
  lwz       r9, -0x4(r1);
  lbz       r11, 0x1E(r12);
  cmpwi     r11, 0;
  beq-      loc_0x21C;
  lbz       r9, 0x14(r12);

loc_0x21C:
  stb       r9, 0xB8(r29);
  stb       r9, 0x2BC(r29);

loc_0x224:
  lbz       r6, 0x731(r10);
  li        r7, 0;
  li        r9, -0xF;
  lbz       r11, 0x732(r10);
  cmpwi     r11, 0;
  bne-      loc_0x244;
  li        r7, 0xFF;
  li        r9, 0xF;

loc_0x244:
  cmpw      r6, r7;
  beq-      loc_0x254;
  add       r6, r6, r9;
  b         loc_0x258;

loc_0x254:
  xori      r11, r11, 0x1;

loc_0x258:
  stb       r11, 0x732(r10);
  li        r0, 0x1;
  stb       r0, 0xBB(r29);
  lwz       r9, 0x1C0(r28);
  stb       r0, 0xBB(r9);
  li        r31, 0;
  lfs       f0, 0x624(r10);
  lfs       f1, 0x10(r8);
  lfs       f2, 0xC(r8);
  fsubs     f0, f0, f2;
  fcmpo     cr0, f0, f1;
  bge-      loc_0x28C;

loc_0x288:
  fsubs     f0, f0, f0;

loc_0x28C:
  stfs      f0, 0x624(r10);
  stb       r0, 0x69F(r10);
  stb       r6, 0x731(r10);
  lwz       r0, 0xF4(r27);
  cmpwi     r0, 0x2;
  beq-      loc_0x2D8;
  li        r4, 0xFF;
  cmpwi     r0, 0;
  beq-      loc_0x2C0;
  stb       r4, 0x729(r10);
  stb       r4, 0x541(r10);
  stb       r4, 0x549(r10);
  b         loc_0x2CC;

loc_0x2C0:
  stb       r4, 0x725(r10);
  stb       r4, 0x53D(r10);
  stb       r4, 0x545(r10);

loc_0x2CC:
  stb       r4, 0x54B(r10);
  li        r4, 0x8C;
  stb       r4, 0x3EA(r10);

loc_0x2D8:
  lfs       f1, 0x0(r30);
  b KartStatusEnd3;
  )
}

asmFunc KartStatusOnMinimap4() {
  ASM(
    nofralloc;
  lwz       r0, 0x8(r3);
  andis.    r12, r0, 0x4;
  beq-      loc_0x10;
  ori       r0, r0, 0x1;

loc_0x10:
  blr;
  )
}

kmRuntimeUse(0x807EB38C);
kmRuntimeUse(0x807EB9CC);
kmRuntimeUse(0x807EB6E0);
kmRuntimeUse(0x807EB1B0);
kmRuntimeUse(0x807EB298);
kmRuntimeUse(0x807EB550);

static void MinimapEffectsInit() {
    kmRuntimeWrite32A(0x807EB298, 0x48000034);
    kmRuntimeWrite32A(0x807EB550, 0x38000000);
    kmRuntimeCallA(0x807EB1B0, KartStatusesOnMinimap1);
    kmRuntimeBranchA(0x807EB6E0, KartStatusOnMinimap2);
    kmRuntimeBranchA(0x807EB9CC, KartStatusOnMinimap3);
    kmRuntimeCallA(0x807EB38C, KartStatusOnMinimap4);
}
BootHook MinimapEffectsHook(MinimapEffectsInit, 5);

} //namespace UI
} //namespace Pulsar
