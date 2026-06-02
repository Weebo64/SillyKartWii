#include <kamek.hpp>
namespace Pulsar {
namespace Race {
// Starting VR/BR (1000)
kmWrite32(0x80548330, 0x38A00000 + 1000); // thank you toadette hack fan

//VR/BR Limit Extension (30.000)
kmWrite16(0x8052D286, 0x00007530);
kmWrite16(0x8052D28E, 0x00007530);
kmWrite16(0x8064F6DA, 0x00007530);
kmWrite16(0x8064F6E6, 0x00007530);
kmWrite16(0x8085654E, 0x00007530);
kmWrite16(0x80856556, 0x00007530);
kmWrite16(0x8085C23E, 0x00007530);
kmWrite16(0x8085C246, 0x00007530);
kmWrite16(0x8064F76A, 0x00007530);
kmWrite16(0x8064F776, 0x00007530);
kmWrite16(0x808565BA, 0x00007530);
kmWrite16(0x808565C2, 0x00007530);
kmWrite16(0x8085C322, 0x00007530);
kmWrite16(0x8085C32A, 0x00007530);

//VR System Changes [MrBean35000vr]
//Multiply VR difference by 2 [Winner]
asmFunc GetVRScaleWin() {
    ASM(
  li r5, 2;
  divw r3, r3, r5;
  extsh r3, r3;
    )
}
kmCall(0x8052D150, GetVRScaleWin);

//Cap VR loss from one victorious opponent between -1 and -3.
asmFunc GetCapVRLoss() {
    ASM(
  lwz       r3, 0x14(r1);
  cmpwi     r3, -3;
  bge       0f;
  li        r3, -3;
  b         1f;
  0:;
  cmpwi     r3, -1;
  ble       1f;
  li        r3, -1;
  1:;
    )
}
kmCall(0x8052D260, GetCapVRLoss);

//Cap VR gain from one defeated opponent between 4 and 7.
asmFunc GetCapVRGain() {
    ASM(
   lwz       r3, 0x14(r1);
   cmpwi     r3, 4;
   bge       0f;
   li        r3, 4;
   b         1f;
   0:;
   cmpwi     r3, 7;
   ble       1f;
   li        r3, 7;
   1:;
    )
}
kmCall(0x8052D1B0, GetCapVRGain);

}//namespace Race
}//namespace Pulsar
