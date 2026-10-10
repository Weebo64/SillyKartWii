#include <kamek.hpp>
#include <SillyKartWii.hpp>

namespace Pulsar {
namespace Characters {

//Everyone has the same allkart [Toadette Hack Fan] unnötig amk
// kmWrite16(0x80890BE4, '--');
// kmWrite16(0x80890C00, '--');
// kmWrite16(0x80890C4D, '--');
// kmWrite16(0x80890C69, '--');
// kmWrite16(0x80890C88, '--');
// kmWrite16(0x80890CA4, '--');

//NameTag Fix [Toadette Hack Fan]
kmWrite8(0x808A9F2D, 0x63);

//Team Preview Fix [Toadette Hack Fan] crasht amk
// kmWrite8(0x808A9326, 0x55);

//Results Fix [Toadette Hack Fan]
kmWrite8(0x808AA455, 0x53);
kmWrite8(0x808AA45E, 0x53);
kmWrite8(0x808AA467, 0x53);
kmWrite8(0x808AA657, 0x53);
kmWrite8(0x8089A52F, 0x58);
kmWrite8(0x808995DA, 0x58);

//Award Fix [Toadette Hack Fan]
kmWrite8(0x80892BE5, 0x42);
kmWrite8(0x80892BF2, 0x42);
kmWrite8(0x80892D1F, 0x42);
kmWrite8(0x80892D2C, 0x42);

void CharacterLayers() {
  U16_ROSALINA_MODEL = 'rs';
  
  if(U16_MISSION_MODE_FIX == 0) {
    u8 rosalinaSettingValue = Settings::Mgr::Get().GetSettingValue(Settings::SETTINGSTYPE_CHARACTERS, SETTINGS_ROSALINA);

    if(rosalinaSettingValue == 1) {
      U16_ROSALINA_MODEL = 'wb';
    }
    
  }
}
static PageLoadHook CHARACTERLAYERS(CharacterLayers);


}//namespace Characters
}//namespace Pulsar