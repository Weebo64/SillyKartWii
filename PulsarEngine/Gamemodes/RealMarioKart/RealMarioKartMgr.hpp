#ifndef _PUL_REALMARIOKART_MGR_
#define _PUL_REALMARIOKART_MGR_

#include <kamek.hpp>
#include <PulsarSystem.hpp>

namespace Pulsar {
namespace RealMarioKart {

class Mgr {
public:
    static void Create();
    static bool IsActive();
    
    Mgr();
    ~Mgr();
};

} // namespace RealMarioKart
} // namespace Pulsar

#endif
