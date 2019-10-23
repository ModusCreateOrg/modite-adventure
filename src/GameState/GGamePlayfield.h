#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

//#include "Game.h"

#include <CreativeEngine.h>

class GGameState;

const TFloat GRAVITY = 0.25; // for falling, arrow dropping, etc.

// Sprite box is adjusted by these (smaller) to make walking (avoid wall collision) more generous
const TFloat FLOOR_ADJUST_LEFT = 8;
const TFloat FLOOR_ADJUST_TOP = 22;
const TFloat FLOOR_ADJUST_RIGHT = 8;
const TFloat FLOOR_ADJUST_BOTTOM = 2;

#include "object_layer_attributes.h"

// these match the codes set for tiles in Pro Motion:

//// MAP LAYER
// NOTE: Map layer is only used for tile numbers to render.  The codes are ignored.

//// MAP ATTRIBUTE LAYER
const TUint16 ATTR_FLOOR = 0;
const TUint16 ATTR_WALL = 1;
const TUint16 ATTR_LEDGE = 2;

//const TUint16 ATTR_PROJECTILE_ARROW = 25;  // not sure this is going to be used like a GEnemyProcess

//// OBJECT LAYER ATTRIBUTES
const TUint16 ATTR_GONE = 0; // thing is no longer in the world
const TUint16 ATTR_STONE_STAIRS_UP = 1;
const TUint16 ATTR_STONE_STAIRS_DOWN = 2;
const TUint16 ATTR_WOOD_STAIRS_UP = 3;
const TUint16 ATTR_WOOD_STAIRS_DOWN = 4;
const TUint16 ATTR_CRATE = 5;
const TUint16 ATTR_CHEST = 6;
const TUint16 ATTR_SPIKES = 7;
const TUint16 ATTR_WOOD_DOOR_H = 8;   // horizontal
const TUint16 ATTR_WOOD_DOOR_V = 9;   // vertical
const TUint16 ATTR_METAL_DOOR_H = 10; // horizontal
const TUint16 ATTR_METAL_DOOR_V = 11; // vertical
const TUint16 ATTR_LEVER = 12;        // tri state lever
const TUint16 ATTR_FLOOR_SWITCH = 13;
const TUint16 ATTR_FLOOR_SWITCH_WOOD = 14;
const TUint16 ATTR_PLAYER = 16;
const TUint16 ATTR_SPIDER = 17;
const TUint16 ATTR_BAT = 18;
const TUint16 ATTR_GOBLIN = 19;
const TUint16 ATTR_GOBLIN_SNIPER = 20;
const TUint16 ATTR_ORC = 21;
const TUint16 ATTR_RAT = 22;
const TUint16 ATTR_SLIME = 23;
const TUint16 ATTR_TROLL = 24;
const TUint16 ATTR_CHEST_OPEN = 25;
const TUint16 ATTR_CRATE_GONE = 26;
const TUint16 ATTR_ITEM = 27; // this is used for ObjectProgram to indicate an item is present
//
const TUint16 ATTR_KEEP = 32767;  // pass this to EndProgram to keep existing OBJECT_LAYER value

//// OBJECT ATTRIBUTE LAYER ATTRIBUTES
// Note: these attributes are unique per thing
// That is, stairs has its own 0-0xffff attributes and crate has its own, and so on.
// Some things, like a key, might be in a crate or simply on the floor, so those have
// to be considered/globally unique.

// stairs (destinations)
const TUint16 ATTR_LEVEL1 = 1;
const TUint16 ATTR_LEVEL2 = 2;
const TUint16 ATTR_LEVEL3 = 3;
const TUint16 ATTR_LEVEL4 = 4;
const TUint16 ATTR_LEVEL5 = 5;
const TUint16 ATTR_LEVEL6 = 6;
const TUint16 ATTR_LEVEL7 = 7;
const TUint16 ATTR_LEVEL8 = 8;
const TUint16 ATTR_LEVEL9 = 9;
const TUint16 ATTR_LEVEL10 = 10;
const TUint16 ATTR_LEVEL_OVERWORLD = 99;

// items (can be inside crates, chests, inventory)
// RED = LEFT, BLUE = MAGIC
// SEE
const TUint16 ATTR_EMPTY = 0;
const TUint16 ATTR_BLUE_SPELLBOOK = 1;
const TUint16 ATTR_RED_SPELLBOOK = 2;
const TUint16 ATTR_GREEN_SPELLBOOK = 3;
const TUint16 ATTR_YELLOW_SPELLBOOK = 4;
const TUint16 ATTR_SILVER_KEY = 5;
const TUint16 ATTR_GOLD_KEY = 6;
const TUint16 ATTR_BLUE_RING = 7;
const TUint16 ATTR_RED_RING = 8;
const TUint16 ATTR_GREEN_RING = 9;
const TUint16 ATTR_YELLOW_RING = 10;
const TUint16 ATTR_BOOTS = 11;
const TUint16 ATTR_GLOVES = 12;
const TUint16 ATTR_SWORD = 13;
const TUint16 ATTR_BLUE_AMULET = 14;
const TUint16 ATTR_RED_AMULET = 15;
const TUint16 ATTR_GREEN_AMULET = 16;
const TUint16 ATTR_YELLOW_AMULET = 17;
const TUint16 ATTR_RED_POTION1 = 18;
const TUint16 ATTR_RED_POTION2 = 19;
const TUint16 ATTR_BLUE_POTION1 = 20;
const TUint16 ATTR_BLUE_POTION2 = 21;
const TUint16 ATTR_EMPTY_BOTTLE = 22; // empty bottle
const TUint16 ATTR_BLUE_BOTTLE1 = 23; // 25% full
const TUint16 ATTR_BLUE_BOTTLE2 = 24; // 50% full
const TUint16 ATTR_BLUE_BOTTLE3 = 25; // 75% full
const TUint16 ATTR_BLUE_BOTTLE4 = 26; // 100% full
const TUint16 ATTR_RED_BOTTLE1 = 27;  // 25% full
const TUint16 ATTR_RED_BOTTLE2 = 28;  // 50% full
const TUint16 ATTR_RED_BOTTLE3 = 27;  // 75% full
const TUint16 ATTR_RED_BOTTLE4 = 28;  // 100% full

// these match the tile numbers in the tileset that are used for sprites
// TODO: get these from ENVIRONMENT_SLOT instead!
//const TUint16 IMG_OBJ_STAIRS_DOWN = 187;  // don't collide with this one (only one sprite)
//const TUint16 IMG_OBJ_STAIRS_UP1  = 189;  // don't collide with this one
//const TUint16 IMG_OBJ_STAIRS_UP2  = 188;

class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId);

  ~GGamePlayfield() OVERRIDE;

public:
  TUint16 GetAttribute(TFloat aWorldX, TFloat aWorldY) {
    return TUint16(GetCell(aWorldX, aWorldY) >> 16);
  }

  TBool IsWall(TFloat aWorldX, TFloat aWorldY) {
    return GetAttribute(aWorldX, aWorldY) == ATTR_WALL;
  }

  TBool IsFloor(TFloat aWorldX, TFloat aWorldY) {
    return GetAttribute(aWorldX, aWorldY) == ATTR_FLOOR;
  }

public:
  TBool mGroupState[16], mGroupDone[16];

protected:
  GGameState *mGameState;
};

#endif //MODITE_GGAMEPLAYFIELD_H
