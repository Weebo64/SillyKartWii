#include <kamek.hpp>
#include <PulsarSystem.hpp>
#include <Config.hpp>
#include <Settings/SettingsParam.hpp>

namespace Pulsar {

namespace Settings {

u8 Params::radioCount[Params::pageCount] ={
    5, 5, 3, 5, 2, 0

};
u8 Params::scrollerCount[Params::pageCount] ={ 1, 1, 1, 0, 2, 0 };

u8 Params::buttonsPerPagePerRow[Params::pageCount][Params::maxRadioCount] =
{
    { 2, 2, 3, 2, 2, 0, 0, 0 },
    { 2, 2, 2, 2, 3, 0, 0, 0 },
    { 2, 4, 2, 0, 0, 0, 0, 0 },
    { 3, 3, 2, 2, 2, 0, 0, 0 },
    { 2, 2, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

u8 Params::optionsPerPagePerScroller[Params::pageCount][Params::maxScrollerCount] =
{
    { 5, 0, 0, 0, 0, 0, 0, 0},
    { 4, 0, 0, 0, 0, 0, 0, 0},
    { 7, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 4, 4, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
};

}//namespace Settings
}//namespace Pulsar



