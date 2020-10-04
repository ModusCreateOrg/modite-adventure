#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

//#include "Game.h"

#include <CreativeEngine.h>

class GGameState;

const TFloat GRAVITY = 15.0 / FRAMES_PER_SECOND; // for falling, arrow dropping, etc.

#include "object_layer_attributes.h"

// these match the codes set for tiles in Pro Motion:

//// MAP LAYER
// NOTE: Map layer is only used for tile numbers to render.  The codes are ignored.

//// MAP ATTRIBUTE LAYER
const TUint16 ATTR_ROTATE_90 = 1u << 0u;
const TUint16 ATTR_ROTATE_180 = 1u << 1u;
const TUint16 ATTR_INVERT = 1u << 2u;
const TUint16 ATTR_FULL_FLOOR = 0;
const TUint16 ATTR_LEDGE = 1;
const TUint16 ATTR_THIN_WALL = 2;
const TUint16 ATTR_CORNER_IN = 3;
const TUint16 ATTR_CORNER_OUT = 4;
const TUint16 ATTR_CORNER_DIAGONAL = 5;
const TUint16 ATTR_HORIZONTAL_DOOR_WALL = 6;
const TUint16 ATTR_VERTICAL_DOOR_WALL = 7;

//const TUint16 ATTR_PROJECTILE_ARROW = 25;  // not sure this is going to be used like a GEnemyProcess

//// OBJECT LAYER VALUES
const TUint16 ATTR_GONE = 0; // thing is no longer in the world
const TUint16 ATTR_STONE_STAIRS_UP = 1;
const TUint16 ATTR_STONE_STAIRS_DOWN = 2;
const TUint16 ATTR_WOOD_STAIRS_UP = 3;
const TUint16 ATTR_WOOD_STAIRS_DOWN = 4;
const TUint16 ATTR_POT = 5;
const TUint16 ATTR_CHEST = 6;
const TUint16 ATTR_SPIKES = 7;
const TUint16 ATTR_WOOD_DOOR_H = 8;   // horizontal
const TUint16 ATTR_WOOD_DOOR_V = 9;   // vertical
const TUint16 ATTR_METAL_GATE_H = 10; // horizontal
const TUint16 ATTR_METAL_GATE_V = 11; // vertical
const TUint16 ATTR_LEVER = 12;        // tri state lever
const TUint16 ATTR_FLOOR_SWITCH = 13;
const TUint16 ATTR_FLOOR_SWITCH_WOOD = 14;
const TUint16 ATTR_PLAYER_IN1 = 100;
const TUint16 ATTR_PLAYER_IN2 = 101;
const TUint16 ATTR_SPIDER = 17;
const TUint16 ATTR_BAT = 18;
const TUint16 ATTR_GOBLIN = 19;
const TUint16 ATTR_GOBLIN_SNIPER = 20;
const TUint16 ATTR_ORC = 21;
const TUint16 ATTR_RAT = 22;
const TUint16 ATTR_SLIME = 23;
const TUint16 ATTR_TROLL = 24;
const TUint16 ATTR_CHEST_OPEN = 25;
const TUint16 ATTR_POT_GONE = 26;
const TUint16 ATTR_DUNGEON_ENTRANCE = 27;
const TUint16 ATTR_DUNGEON_EXIT_FROM_LEVEL_1 = 28;
const TUint16 ATTR_TURRET = 29;
const TUint16 ATTR_SPAWN_SMALL_HEALTH = 30;
const TUint16 ATTR_SPAWN_LARGE_HEALTH = 31;
const TUint16 ATTR_SPAWN_SMALL_MANA = 32;
const TUint16 ATTR_SPAWN_LARGE_MANA = 33;
const TUint16 ATTR_SPAWN_HEART = 34;
// mid bosses
const TUint16 ATTR_MID_BOSS_EARTH = 1001;
const TUint16 ATTR_MID_BOSS_WATER = 1002;
const TUint16 ATTR_MID_BOSS_FIRE = 1003;
const TUint16 ATTR_MID_BOSS_ENERGY = 1004;
const TUint16 ATTR_WIZARD_EARTH = 1005;
const TUint16 ATTR_WIZARD_WATER = 1006;
const TUint16 ATTR_WIZARD_FIRE = 1007;
const TUint16 ATTR_WIZARD_ENERGY = 1008;
const TUint16 ATTR_FINAL_BOSS = 1009;
const TUint16 ATTR_FINAL_BOSS_EARTH = 1005;
const TUint16 ATTR_FINAL_BOSS_WATER = 1006;
const TUint16 ATTR_FINAL_BOSS_FIRE = 1007;
const TUint16 ATTR_FINAL_BOSS_ENERGY = 1008;



//
const TUint16 ATTR_KEEP = 32767;  // pass this to EndProgram to keep existing OBJECT_LAYER value

//// OBJECT ATTRIBUTE LAYER VALUES
// Note: these values are unique per thing
// That is, stairs has its own 0-0xffff attributes and crate has its own, and so on.
// Some things, like a key, might be in a crate or simply on the floor, so those have
// to be considered/globally unique.

// stairs (destinations)
const TUint16 ATTR_OW_LEVEL_ENTRANCE = 99;

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
const TUint16 ATTR_HEART = 22;

const TUint16 ATTR_EMPTY_BOTTLE = 23; // empty bottle
const TUint16 ATTR_BLUE_BOTTLE1 = 24; // 25% full
const TUint16 ATTR_BLUE_BOTTLE2 = 25; // 50% full
const TUint16 ATTR_BLUE_BOTTLE3 = 26; // 75% full
const TUint16 ATTR_BLUE_BOTTLE4 = 27; // 100% full
const TUint16 ATTR_RED_BOTTLE1 = 28;  // 25% full
const TUint16 ATTR_RED_BOTTLE2 = 29;  // 50% full
const TUint16 ATTR_RED_BOTTLE3 = 30;  // 75% full
const TUint16 ATTR_RED_BOTTLE4 = 31;  // 100% full

const TInt MOSAIC_DURATION = 0.5 * FRAMES_PER_SECOND;
const TInt MOSAIC_INTENSITY = 20;

const TInt WALL_THICKNESS = 12; // minimum thickness of walls

class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId);

  ~GGamePlayfield() OVERRIDE;

public:
  void Render() OVERRIDE;

  void StartMosaicIn();

  void StartMosaicOut();

  TBool MosaicActive() {
    return mMosaicTimer > 1;
  }

  TBool MosaicDone() {
    return mMosaicTimer == 1;
  };

  void Restore();

  void DumpObjectProgram() {
    for (TInt i=0; i<mObjectCount; i++) {
      mObjectProgram[i].Dump(i);
    }
  }

  void DumpMap();
  void DumpMapAttributes();

public:
  TUint16 GetAttribute(TFloat aWorldX, TFloat aWorldY) {
    return TUint16(GetCell(aWorldX, aWorldY) >> 16);
  }

  TUint16 GetAttribute(TInt aRow, TInt aCol) {
    return TUint16(GetCell(aRow, aCol) >> 16);
  }

  TBool IsFloor(TFloat aWorldX, TFloat aWorldY) {
    const TUint16 attr = GetAttribute(aWorldX, aWorldY);
    const TInt col = TInt(FLOOR(aWorldX / TILESIZE)),
            row = TInt(FLOOR(aWorldY / TILESIZE));
    TInt x = TInt(aWorldX) % TILESIZE, y = TInt(aWorldY) % TILESIZE, tmp;
    if (row >= 0 && row <= mMapHeight - 1 && col >= 0 && col <= mMapWidth - 1) {
      TBool inverted = attr & ATTR_INVERT;
      if (attr & ATTR_ROTATE_90) {
        tmp = x;
        x = y;
        y = TILESIZE - tmp;
      }
      if (attr & ATTR_ROTATE_180) {
        x = TILESIZE - x;
        y = TILESIZE - y;
      }
      switch (attr >> 3u) {
        case ATTR_FULL_FLOOR:
        default:
          return !inverted;
        case ATTR_THIN_WALL:
        case ATTR_LEDGE:
          return inverted ^ (x > WALL_THICKNESS);
        case ATTR_CORNER_IN:
          return inverted ^ (x > WALL_THICKNESS && y > WALL_THICKNESS);
        case ATTR_CORNER_OUT:
          return inverted ^ (x > WALL_THICKNESS || y > WALL_THICKNESS);
        case ATTR_CORNER_DIAGONAL:
          return inverted ^ (x + y < TILESIZE - WALL_THICKNESS);
        case ATTR_HORIZONTAL_DOOR_WALL:
          return y < 16;
        case ATTR_VERTICAL_DOOR_WALL:
          return x < 8 || x > 24;
      }
    }
    else {
      return (row == -1 && y > -WALL_THICKNESS) ||
             (row == mMapHeight && y <= WALL_THICKNESS) ||
             (col == -1 && x > -WALL_THICKNESS) ||
             (col == mMapWidth && x <= WALL_THICKNESS);
    }
  }

  TBool IsLedge(TFloat aWorldX, TFloat aWorldY) {
    return GetAttribute(aWorldX, aWorldY) >> 3u == ATTR_LEDGE;
  }

public:
  TBool mGroupState[16], mGroupDone[16];

public:
  void WriteToStream(BMemoryStream &aStream);

  void ReadFromStream(BMemoryStream &aStream);

protected:
  GGameState *mGameState;
  TUint32 mTileMapId;
  TInt mMosaicTimer;
  TBool mMosaicIn;
  TRGB mSavedPalette[256];
};

#endif //MODITE_GGAMEPLAYFIELD_H
