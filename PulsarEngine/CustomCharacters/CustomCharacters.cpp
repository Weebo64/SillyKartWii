#include <kamek.hpp>
#include <CustomCharacters/CustomCharacters.hpp>

namespace Pulsar {
namespace CustomCharacters {

// Character Loader Patches [by Toadette Hack Fan]

void CharacterLayers() {
  U16_MARIO_MODEL = 'mr';
  U16_BABY_PEACH_MODEL = 'bp';
  U16_WALUIGI_MODEL = 'wl';
  U16_BOWSER_MODEL = 'kp';
  U16_BABY_DAISY_MODEL = 'bd';
  U16_DRY_BONES_MODEL = 'ka';
  U16_BABY_MARIO_MODEL = 'bm';
  U16_LUIGI_MODEL = 'lg';
  U16_TOAD_MODEL = 'ko';
  U16_DONKEY_KONG_MODEL = 'dk';
  U16_YOSHI_MODEL = 'ys';
  U16_WARIO_MODEL = 'wr';
  U16_BABY_LUIGI_MODEL = 'bl';
  U16_TOADETTE_MODEL = 'kk';
  U16_KOOPA_TROOPA_MODEL = 'nk';
  U16_DAISY_MODEL = 'ds';
  U16_PEACH_MODEL = 'pc';
  U16_BIRDO_MODEL = 'ca';
  U16_DIDDY_KONG_MODEL = 'dd';
  U16_KING_BOO_MODEL = 'kt';
  U16_BOWSER_JR_MODEL = 'jr';
  U16_DRY_BOWSER_MODEL = 'bk';
  U16_FUNKY_KONG_MODEL = 'fk';
  
  // Replace Rosalina with WeebOwO
  U16_ROSALINA_MODEL = 'rs';
  
  U8_MARIO_ICON = 'c';
  U8_BABY_PEACH_ICON = 'c';
  U8_WALUIGI_ICON = 'c';
  U8_BOWSER_ICON = 'c';
  U8_BABY_DAISY_ICON = 'c';
  U8_DRY_BONES_ICON = 'c';
  U8_BABY_MARIO_ICON = 'c';
  U8_LUIGI_ICON = 'c';
  U8_TOAD_ICON = 'c';
  U8_DONKEY_KONG_ICON = 'c';
  U8_YOSHI_ICON = 'c';
  U8_WARIO_ICON = 'c';
  U8_BABY_LUIGI_ICON = 'c';
  U8_TOADETTE_ICON = 'c';
  U8_KOOPA_TROOPA_ICON = 'c';
  U8_DAISY_ICON = 'c';
  U8_PEACH_ICON = 'c';
  U8_BIRDO_ICON = 'c';
  U8_DIDDY_KONG_ICON = 'c';
  U8_KING_BOO_ICON = 'c';
  U8_BOWSER_JR_ICON = 'c';
  U8_DRY_BOWSER_ICON = 'c';
  U8_FUNKY_KONG_ICON = 'c';
  U8_ROSALINA_ICON = 'c';
}

static PageLoadHook CHARACTERLAYERS(CharacterLayers);

} // namespace CustomCharacters
} // namespace Pulsar
