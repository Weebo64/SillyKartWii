#ifndef _PUL_FRMGR_
#define _PUL_FRMGR_

#include <kamek.hpp>
#include <MarioKartWii/Race/RaceData.hpp>
#include <PulsarSystem.hpp>

namespace Pulsar {
namespace FreeRoam {

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
