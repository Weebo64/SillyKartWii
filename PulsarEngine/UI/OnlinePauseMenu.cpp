#include <kamek.hpp>

extern u8 U8_WWS_CHECK;

namespace Pulsar {
namespace UI {

kmWrite32(0x8062ED3C, 0x4BFFFF88);
kmWrite32(0x8062ED90, 0x4BFFFF34);
kmWrite32(0x8062EDE4, 0x4BFFFEE0);
kmWrite32(0x8062EE68, 0x4BFFFE5C);
kmWrite32(0x8062EEEC, 0x4BFFFDD8);
kmWrite32(0x8062EF4C, 0x4BFFFD78);
kmWrite32(0x8062EFAC, 0x4BFFFD18);
kmWrite32(0x8051B560, 0x38000000);
kmWrite32(0x807F074C, 0x80A40000);
kmWrite32(0x807F0750, 0x38000000);
kmWrite32(0x808567B8, 0x38000000);
kmWrite32(0x80859A54, 0x41820014);
kmWrite32(0x80859A58, 0x2C00001F);
kmWrite32(0x80859F54, 0x3800001F);

extern "C" void OnlinePausingEnd1(void*);
asmFunc OnlinePausing1() {
  ASM(
  nofralloc;
  lwz       r12, 0x0(r3);
  cmpwi     r12, 0x68;
  blt-      loc_0x24;
  cmpwi     r12, 0x77;
  bgt-      loc_0x24;
  cmpwi     r0, 0x4;
  beqlr-;
  cmpwi     r0, 0x60;
  beqlr-;

loc_0x24:
  li        r0, 0x1;
  b OnlinePausingEnd1;
  )
}
kmBranch(0x80623054, OnlinePausing1);

extern "C" void OnlinePausingEnd2(void*);
asmFunc OnlinePausing2() {
  ASM(
  nofralloc;
  lwz       r3, 0x0(r28);
  cmpwi     r3, 0x68;
  blt-      loc_0x18;
  cmpwi     r3, 0x77;
  bgt-      loc_0x18;
  li        r3, 0;

loc_0x18:
  b OnlinePausingEnd2;
  )
}
kmBranch(0x80622454, OnlinePausing2);

extern "C" void CreateAndInitPage__7SectionF6PageId(void*);
extern "C" void OnlinePausingEnd3(void*);
asmFunc OnlinePausing3() {
  ASM(
  nofralloc;
  mr        r3, r31;
  li        r4, 0x18;
  lis       r12, CreateAndInitPage__7SectionF6PageId@h;
  ori       r12, r12, CreateAndInitPage__7SectionF6PageId@l;
  mtctr     r12;
  bctrl;
  mr        r3, r31;
  li        r4, 0x2C;
  lis       r12, CreateAndInitPage__7SectionF6PageId@h;
  ori       r12, r12, CreateAndInitPage__7SectionF6PageId@l;
  mtctr     r12;
  bctrl;
  mr        r3, r31;
  b OnlinePausingEnd3;
  )
}
kmBranch(0x8062ECC4, OnlinePausing3);

extern "C" void sInstance__8Raceinfo(void*);
extern "C" void OnlinePausingEnd4(void*);
asmFunc OnlinePausing4() {
  ASM(
  nofralloc;
  lwz       r29, 0x8(r4);
  lis       r12, sInstance__8Raceinfo@ha;
  lwz       r12, sInstance__8Raceinfo@l(r12);
  lwz       r12, 0x28(r12);
  cmpwi     r12, 0x2;
  ble-      loc_0x5C;
  cmpwi     r29, 0x4;
  bne-      loc_0x5C;
  li        r12, 0;
  stb       r12, 0x389(r5);
  li        r12, 0x5;
  stw       r12, 0x8(r4);
  li        r12, 0x1;
  stw       r12, 0x10(r4);
  stwu      r1, -0x80(r1);
  stmw      r3, 0x8(r1);
  mr        r3, r4;
  lwz       r12, 0x0(r4);
  lwz       r12, 0x40(r12);
  mtctr     r12;
  bctrl;
  lmw       r3, 0x8(r1);
  addi      r1, r1, 0x80;

loc_0x5C:
  mr        r4, r29;
  b OnlinePausingEnd4;
  )
}
kmBranch(0x807F0774, OnlinePausing4);

extern "C" void OnlinePausingEnd5(void*);
asmFunc OnlinePausing5() {
  ASM(
  nofralloc;
  li        r5, 0x1;
  lwz       r12, 0x98(r3);
  lwz       r12, 0x124(r12);
  cmpwi     r12, 0x1;
  ble-      loc_0x18;
  li        r0, 0;

loc_0x18:
  b OnlinePausingEnd5;
  )
}
kmBranch(0x80858E24, OnlinePausing5);

extern "C" void OnlinePausingEnd6(void*);
asmFunc OnlinePausing6() {
  ASM(
  nofralloc;
  lwz       r3, 0x0(r3);
  cmpwi     r3, 0x68;
  blt-      loc_0x18;
  cmpwi     r3, 0x77;
  bgt-      loc_0x18;
  li        r3, 0x1E;

loc_0x18:
  b OnlinePausingEnd6;
  )
}
kmBranch(0x80858E2C, OnlinePausing6);

extern "C" void OnlinePausingEnd7(void*);
asmFunc OnlinePausing7() {
  ASM(
  nofralloc;
  lwz       r0, 0x0(r3);
  cmpwi     r0, 0x68;
  blt-      loc_0x18;
  cmpwi     r0, 0x77;
  bgt-      loc_0x18;
  li        r0, 0x1E;

loc_0x18:
  b OnlinePausingEnd7;
  )
}
kmBranch(0x80859058, OnlinePausing7);

extern "C" void OnlinePausingEnd8(void*);
asmFunc OnlinePausing8() {
  ASM(
  nofralloc;
  bne-      loc_0x8;
  li        r31, 0x18;

loc_0x8:
  b OnlinePausingEnd8;
  )
}

kmRuntimeUse(0x808569EC);
void OnlinePausing8Toggle() {
  kmRuntimeWrite32A(0x808569EC, 0x41820174);
  if(U8_WWS_CHECK == 1) kmRuntimeBranchA(0x808569EC, OnlinePausing8);
}
static PageLoadHook ONLINEPAUSE(OnlinePausing8Toggle);

extern "C" void sInstance__Q25RKNet10Controller(void*);
extern "C" void ResetRH1andROOM__Q25RKNet10ControllerFv(void*);
extern "C" void sInstance__10SectionMgr(void*);
extern "C" void OnlinePausingEnd9(void*);
asmFunc OnlinePausing9() {
  ASM(
  nofralloc;
  li        r4, 0x41;
  lis       r29, sInstance__Q25RKNet10Controller@ha;
  lwz       r29, sInstance__Q25RKNet10Controller@l(r29);
  lwz       r0, 0x28(r29);
  cmpwi     r0, 0;
  beq-      loc_0x5C;
  stwu      r1, -0x80(r1);
  stmw      r3, 0x8(r1);
  mr        r3, r29;
  lis       r12, ResetRH1andROOM__Q25RKNet10ControllerFv@h;
  ori       r12, r12, ResetRH1andROOM__Q25RKNet10ControllerFv@l;
  mtctr     r12;
  bctrl;
  lmw       r3, 0x8(r1);
  addi      r1, r1, 0x80;
  li        r4, 0x55;
  lis       r29, sInstance__10SectionMgr@ha;
  lwz       r29, sInstance__10SectionMgr@l(r29);
  lwz       r29, 0x98(r29);
  lwz       r29, 0x124(r29);
  cmpwi     r29, 0x2;
  bne-      loc_0x5C;
  li        r4, 0x5B;

loc_0x5C:
  b OnlinePausingEnd9;
  )
}
kmBranch(0x8085A45C, OnlinePausing9);

extern "C" void OnlinePausingEndA(void*);
asmFunc OnlinePausing10() {
  ASM(
  nofralloc;
  lwz       r12, 0x0(r3);
  lbz       r3, 0x38B(r3);
  cmpwi     r12, 0x68;
  blt-      loc_0x1C;
  cmpwi     r12, 0x77;
  bgt-      loc_0x1C;
  li        r3, 0;

loc_0x1C:
  b OnlinePausingEndA;
  )
}
kmBranch(0x80554E4C, OnlinePausing10);

}//namespace UI
}//namespace Pulsar