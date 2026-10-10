#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <kamek.hpp>
#include <SillyKartWii.hpp>
#include <runtimeWrite.hpp>

namespace Pulsar {
namespace SKWii {

// Allow POWs and Shocks to affect other players on their screens.
asmFunc ItemRainOnlineFix() {
    ASM(
        addi r3, r4, 200;
        lis r12, 0x8000;
        lbz r12, 0x120C(r12);
        cmpwi r12, 0;
        beq loc_0x2C;
        lbz r3, 27(r27);
        cmpwi r3, 0x7;
        beq + loc_0x24;
        cmpwi r3, 0x3;
        beq + loc_0x24;
        cmpwi r3, 0x4;
        beq + loc_0x24;
        addi r3, r4, 0xC8;
        b loc_0x2C;

        loc_0x24 : mr r3, r4;
        b loc_0x2C;

        loc_0x2C:)
}
kmCall(0x8065BB40, ItemRainOnlineFix);

} // namespace SKWii
} // namespace Pulsar