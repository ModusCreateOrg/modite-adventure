#ifndef MODITE_GRESOURCES_H
#define MODITE_GRESOURCES_H

#include <BTypes.h>
#include "Resources.h"

enum {
  FONT_8x8_SLOT,
  FONT_16x16_SLOT,
  BKG_SLOT,
  PLAYER_SLOT,
  // enemies
  PROJECTILE_ARROW_SLOT,
  BAT_SLOT,
  GOBLIN_SLOT,
  GOBLIN_SNIPER_SLOT,
  ORC_SLOT,
  RAT_SLOT,
  SLIME_SLOT,
  SPIDER_SLOT,
  TROLL_SLOT,
  SLOT_MAX,
};

// Palette Remapping
const TUint16 TILEMAP_COLORS = 64;              // number of colors in tilemap/tileset BMP

const TUint16 PLAYER_COLORS  = 32;               // number of colors reserved for player
const TUint16 PLAYER_PALETTE = TILEMAP_COLORS;  // starting color in palette

const TUint16 BAT_COLORS  = 16;
const TUint16 BAT_PALETTE = (PLAYER_PALETTE + PLAYER_COLORS);

const TUint16 GOBLIN_COLORS  = 16;
const TUint16 GOBLIN_PALETTE = (BAT_PALETTE + BAT_COLORS);

const TUint16 GOBLIN_SNIPER_COLORS  = 16;
const TUint16 GOBLIN_SNIPER_PALETTE = (GOBLIN_PALETTE + GOBLIN_COLORS);

const TUint16 ORC_COLORS  = 16;
const TUint16 ORC_PALETTE = (GOBLIN_SNIPER_PALETTE + GOBLIN_SNIPER_COLORS);

const TUint16 RAT_COLORS  = 16;
const TUint16 RAT_PALETTE = (ORC_PALETTE + ORC_COLORS);

const TUint16 SLIME_COLORS  = 16;
const TUint16 SLIME_PALETTE = (RAT_PALETTE + RAT_COLORS);

const TUint16 SPIDER_COLORS  = 16;
const TUint16 SPIDER_PALETTE = (SLIME_PALETTE + SLIME_COLORS);

const TUint16 TROLL_COLORS  = 16;
const TUint16 TROLL_PALETTE = (SPIDER_PALETTE + SPIDER_COLORS);

const TUint16 PROJECTILE_ARROW_COLORS  = 16;
const TUint16 PROJECTILE_ARROW_PALETTE = (TROLL_PALETTE + TROLL_COLORS);

// BRaw slots
enum {
  SONG0_SLOT,
  SONG1_SLOT,
  SONG2_SLOT,
  SONG3_SLOT,
  SONG4_SLOT,
  SONG5_SLOT,
  SONG6_SLOT,
  SONG7_SLOT,
  SONG8_SLOT,
  SONG9_SLOT,
  SONG10_SLOT,
  SONG11_SLOT,
  SFX1_SLOT,
  SFX2_SLOT,
  SFX3_SLOT,
  SFX4_SLOT,
  SFX5_SLOT,
  SFX6_SLOT,
  SFX7_SLOT,
  SFX8_SLOT,
  SFX9_SLOT,
  SFX10_SLOT,
  SFX11_SLOT,
  SFX12_SLOT,
  SFX13_SLOT,
  SFX14_SLOT,
};

// SPLASH_SPRITES.BMP
static const TUint16 IMG_DROP1 = 0;
static const TUint16 IMG_DROP2 = 4;
static const TUint16 IMG_DROP3 = 8;
static const TUint16 IMG_DROP4 = 16;
static const TUint16 IMG_DROP5 = 24;
static const TUint16 IMG_DROP6 = 32;

// PLAYER.BMP
static const TUint16 IMG_IDLE        = 0;
static const TUint16 IMG_SELECTED    = 4;
static const TUint16 IMG_WALK_DOWN   = 8;
static const TUint16 IMG_WALK_RIGHT  = 12;
static const TUint16 IMG_WALK_UP     = 16;
static const TUint16 IMG_SWORD_DOWN  = 20;
static const TUint16 IMG_SWORD_RIGHT = 24;
static const TUint16 IMG_SWORD_UP    = 28;

// CHARA_SPIDER_BMP
static const TUint16 IMG_SPIDER_IDLE         = 0;
static const TUint16 IMG_SPIDER_SELECTED     = 4;
static const TUint16 IMG_SPIDER_WALK_DOWN    = 8;
static const TUint16 IMG_SPIDER_WALK_RIGHT   = 12;
static const TUint16 IMG_SPIDER_WALK_UP      = 16;
static const TUint16 IMG_SPIDER_ATTACK_DOWN  = 20;
static const TUint16 IMG_SPIDER_ATTACK_RIGHT = 24;
static const TUint16 IMG_SPIDER_ATTACK_UP    = 28;
static const TUint16 IMG_SPIDER_DAMAGE_DOWN  = 32;
static const TUint16 IMG_SPIDER_DAMAGE_RIGHT = 36;
static const TUint16 IMG_SPIDER_DAMAGE_UP    = 40;

// CHARA_BAT_BMP
static const TUint16 IMG_BAT_IDLE         = 0;
static const TUint16 IMG_BAT_SELECTED     = 4;
static const TUint16 IMG_BAT_WALK_DOWN    = 8;
static const TUint16 IMG_BAT_WALK_RIGHT   = 12;
static const TUint16 IMG_BAT_WALK_UP      = 16;
static const TUint16 IMG_BAT_ATTACK_DOWN  = 20;
static const TUint16 IMG_BAT_ATTACK_RIGHT = 24;
static const TUint16 IMG_BAT_ATTACK_UP    = 28;
static const TUint16 IMG_BAT_DAMAGE_DOWN  = 32;
static const TUint16 IMG_BAT_DAMAGE_RIGHT = 36;
static const TUint16 IMG_BAT_DAMAGE_UP    = 40;

// CHARA_GOBLIN_BMP
static const TUint16 IMG_GOBLIN_IDLE         = 0;
static const TUint16 IMG_GOBLIN_SELECTED     = 4;
static const TUint16 IMG_GOBLIN_WALK_DOWN    = 8;
static const TUint16 IMG_GOBLIN_WALK_RIGHT   = 12;
static const TUint16 IMG_GOBLIN_WALK_UP      = 16;
static const TUint16 IMG_GOBLIN_ATTACK_DOWN  = 20;
static const TUint16 IMG_GOBLIN_ATTACK_RIGHT = 24;
static const TUint16 IMG_GOBLIN_ATTACK_UP    = 28;
static const TUint16 IMG_GOBLIN_DAMAGE_DOWN  = 32;
static const TUint16 IMG_GOBLIN_DAMAGE_RIGHT = 36;
static const TUint16 IMG_GOBLIN_DAMAGE_UP    = 40;

// CHARA_GOBLIN_SNIPER_BMP
static const TUint16 IMG_GOBLIN_SNIPER_IDLE         = 0;
static const TUint16 IMG_GOBLIN_SNIPER_SELECTED     = 4;
static const TUint16 IMG_GOBLIN_SNIPER_WALK_DOWN    = 8;
static const TUint16 IMG_GOBLIN_SNIPER_WALK_RIGHT   = 12;
static const TUint16 IMG_GOBLIN_SNIPER_WALK_UP      = 16;
static const TUint16 IMG_GOBLIN_SNIPER_ATTACK_DOWN  = 20;
static const TUint16 IMG_GOBLIN_SNIPER_ATTACK_RIGHT = 24;
static const TUint16 IMG_GOBLIN_SNIPER_ATTACK_UP    = 28;
static const TUint16 IMG_GOBLIN_SNIPER_DAMAGE_DOWN  = 32;
static const TUint16 IMG_GOBLIN_SNIPER_DAMAGE_RIGHT = 36;
static const TUint16 IMG_GOBLIN_SNIPER_DAMAGE_UP    = 40;

// CHARA_ORC_BMP
static const TUint16 IMG_ORC_IDLE         = 0;
static const TUint16 IMG_ORC_SELECTED     = 4;
static const TUint16 IMG_ORC_WALK_DOWN    = 8;
static const TUint16 IMG_ORC_WALK_RIGHT   = 12;
static const TUint16 IMG_ORC_WALK_UP      = 16;
static const TUint16 IMG_ORC_ATTACK_DOWN  = 20;
static const TUint16 IMG_ORC_ATTACK_RIGHT = 24;
static const TUint16 IMG_ORC_ATTACK_UP    = 28;
static const TUint16 IMG_ORC_DAMAGE_DOWN  = 32;
static const TUint16 IMG_ORC_DAMAGE_RIGHT = 36;
static const TUint16 IMG_ORC_DAMAGE_UP    = 40;

// CHARA_RAT_BMP
static const TUint16 IMG_RAT_IDLE         = 0;
static const TUint16 IMG_RAT_SELECTED     = 4;
static const TUint16 IMG_RAT_WALK_DOWN    = 8;
static const TUint16 IMG_RAT_WALK_RIGHT   = 12;
static const TUint16 IMG_RAT_WALK_UP      = 16;
static const TUint16 IMG_RAT_ATTACK_DOWN  = 20;
static const TUint16 IMG_RAT_ATTACK_RIGHT = 24;
static const TUint16 IMG_RAT_ATTACK_UP    = 28;
static const TUint16 IMG_RAT_DAMAGE_DOWN  = 32;
static const TUint16 IMG_RAT_DAMAGE_RIGHT = 36;
static const TUint16 IMG_RAT_DAMAGE_UP    = 40;

// CHARA_SLIME_BMP
static const TUint16 IMG_SLIME_IDLE         = 0;
static const TUint16 IMG_SLIME_SELECTED     = 4;
static const TUint16 IMG_SLIME_WALK_DOWN    = 8;
static const TUint16 IMG_SLIME_WALK_RIGHT   = 12;
static const TUint16 IMG_SLIME_WALK_UP      = 16;
static const TUint16 IMG_SLIME_ATTACK_DOWN  = 20;
static const TUint16 IMG_SLIME_ATTACK_RIGHT = 24;
static const TUint16 IMG_SLIME_ATTACK_UP    = 28;
static const TUint16 IMG_SLIME_DAMAGE_DOWN  = 32;
static const TUint16 IMG_SLIME_DAMAGE_RIGHT = 36;
static const TUint16 IMG_SLIME_DAMAGE_UP    = 40;

// CHARA_TROLL_BMP
static const TUint16 IMG_TROLL_IDLE         = 0;
static const TUint16 IMG_TROLL_SELECTED     = 4;
static const TUint16 IMG_TROLL_WALK_DOWN    = 8;
static const TUint16 IMG_TROLL_WALK_RIGHT   = 12;
static const TUint16 IMG_TROLL_WALK_UP      = 16;
static const TUint16 IMG_TROLL_ATTACK_DOWN  = 20;
static const TUint16 IMG_TROLL_ATTACK_RIGHT = 24;
static const TUint16 IMG_TROLL_ATTACK_UP    = 28;
static const TUint16 IMG_TROLL_DAMAGE_DOWN  = 32;
static const TUint16 IMG_TROLL_DAMAGE_RIGHT = 36;
static const TUint16 IMG_TROLL_DAMAGE_UP    = 40;

static const TUint8 COLOR_TEXT_SHADOW = 253;
static const TUint8 COLOR_TEXT_BG     = 254;
static const TUint8 COLOR_TEXT        = 255;

#define MAX_BBITMAP 5

#endif //MODITE_GRESOURCES_H
