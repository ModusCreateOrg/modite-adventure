#ifndef MODITE_GRESOURCES_H
#define MODITE_GRESOURCES_H

#include <BTypes.h>
#include "Resources.h"

enum {
    BKG_SLOT,
    MAP_SLOT,
    CODES_SLOT,
    PLAYER_SLOT,
    FONT_8x8_SLOT,
    FONT_16x16_SLOT,
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

const TUint8 COLOR_TEXT = 8;

#define MAX_BBITMAP 4

#endif //MODITE_GRESOURCES_H
