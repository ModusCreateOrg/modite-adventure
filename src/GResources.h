#ifndef MODITE_GRESOURCES_H
#define MODITE_GRESOURCES_H

#include <BTypes.h>
#include "Resources.h"

enum {
    BKG_SLOT,
    PLAYER_SLOT,
    FONT_8x8_SLOT,
    FONT_16x16_SLOT,
};

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
static const TUint16 IMG_IDLE = 0;
static const TUint16 IMG_SELECTED = 4;
static const TUint16 IMG_WALK_DOWN = 8;
static const TUint16 IMG_WALK_LEFT_RIGHT = 12;
static const TUint16 IMG_WALK_UP = 16;
static const TUint16 IMG_SWORD_DOWN = 20;
static const TUint16 IMG_SWORD_LEFT_RIGHT = 24;
static const TUint16 IMG_SWORD_UP = 28;

const TUint8 COLOR_TEXT = 8;

#define MAX_BBITMAP 6

#endif //MODITE_GRESOURCES_H
