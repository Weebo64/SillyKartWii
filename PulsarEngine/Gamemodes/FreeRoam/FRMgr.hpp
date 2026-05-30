#ifndef _PUL_FRMGR_
#define _PUL_FRMGR_

#include <kamek.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <PulsarSystem.hpp>

namespace Pulsar {
namespace FreeRoam {

extern u16 U16_FREE_ROAM;
extern u16 WiiInput;
extern u16 GCInput;
extern u16 ClassicInput;

class Mgr {
public:
    static void Create();
    static void Update();
    
    Mgr();
    ~Mgr();
    
    static Mgr* sInstance;
};

}
}

#endif
