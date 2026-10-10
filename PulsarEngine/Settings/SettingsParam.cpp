#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Config.hpp>
#include <Settings/SettingsParam.hpp>

namespace Pulsar {

namespace Settings {

u8 Params::radioCount[Params::pageCount] ={
    5, 5, 3, 5, 2, 6, 5, 4, 1, 0
};
u8 Params::scrollerCount[Params::pageCount] ={ 1, 2, 1, 0, 2, 1, 1, 0, 0, 1 };

u8 Params::buttonsPerPagePerRow[Params::pageCount][Params::maxRadioCount] =
{
    { 3, 2, 3, 2, 2, 0, 0, 0 },
    { 2, 2, 2, 2, 3, 0, 0, 0 },
    { 2, 4, 2, 0, 0, 0, 0, 0 },
    { 3, 3, 2, 2, 2, 0, 0, 0 },
    { 2, 2, 0, 0, 0, 0, 0, 0 },
    { 2, 2, 2, 2, 2, 4, 0, 0 },
    { 2, 2, 2, 2, 2, 0, 0, 0 },
    { 2, 2, 2, 2, 0, 0, 0, 0 },
    { 2, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0, 0, 0, 0, 0, 0, 0 },
};

u8 Params::optionsPerPagePerScroller[Params::pageCount][Params::maxScrollerCount] =
{
    { 5, 0, 0, 0, 0, 0, 0, 0},
    { 4, 2, 0, 0, 0, 0, 0, 0},  // RACE page: SOM scroller (4 options) + Players scroller (2 options)
    { 4, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 4, 4, 0, 0, 0, 0, 0, 0},
    { 3, 0, 0, 0, 0, 0, 0, 0},
    { 15, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 4, 0, 0, 0, 0, 0, 0, 0},
};

}//namespace Settings
}//namespace Pulsar



