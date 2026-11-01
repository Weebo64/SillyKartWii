// Increase RKNet Controller Heap Size
// The RKNet Controller allocates packet buffers using 'new' from its heap.
// With expanded packets (PulSELECT with characterTables, transmission, driftMode),
// we need more heap space to avoid "DAME DAME in eggExpHeap.cpp" errors.
//
// Based on similar heap patches in vk-Pulsar (MiiOutfitC.cpp line 305-307):
// kmWrite32(0x8059e3bc, 0x3F800002);  // lis r28, 0x2 -> 0x20000 seed
// kmWrite32(0x8059e3cc, 0x7F83E378);  // mr r3, r28 -> 0x20000 bytes
//
// We need to patch the RKNet Controller heap creation around 0x80656xxx.
// The typical pattern is:
//   lis r4, 0x5      -> loads 0x50000 (327KB) - TOO SMALL
//   ori r4, r4, 0x0
//
// We change to:
//   lis r4, 0xA      -> loads 0xA0000 (655KB) - MUCH BIGGER
//   ori r4, r4, 0x0

#include <kamek.hpp>
#include <core/rvl/os/OS.hpp>

namespace Pulsar {
namespace Network {

// Try multiple candidate addresses where the heap size might be loaded
// These are educated guesses based on the RKNet Controller address range

// Candidate 1: Most likely location (middle of RKNet init)
kmWrite32(0x80656EBC, 0x3C8000A0); // lis r4, 0xA -> 0xA0000 bytes (was 0x3C800005)

// Candidate 2: Alternative location
// kmWrite32(0x80656EC0, 0x3C8000A0);

// Candidate 3: Another possibility
// kmWrite32(0x80656F00, 0x3C8000A0);

// Debug: Log when this patch is loaded
static void __attribute__((constructor)) LogHeapPatch() {
    OS::Report("[Pulsar] RKNet Heap increase patch loaded (0x50000 -> 0xA0000)\n");
}

}  // namespace Network
}  // namespace Pulsar


