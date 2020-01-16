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
const TUint16 ATTR_FLOOR = 0;
const TUint16 ATTR_WALL = 8;
const TUint16 ATTR_LEDGE = 10;

//const TUint16 ATTR_PROJECTILE_ARROW = 25;  // not sure this is going to be used like a GEnemyProcess

//// OBJECT LAYER VALUES
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
const TUint16 ATTR_CRATE_GONE = 26;
const TUint16 ATTR_DUNGEON_ENTRANCE = 27;
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
const TUint16 ATTR_BLUE_BRACELET = 14;
const TUint16 ATTR_RED_BRACELET = 15;
const TUint16 ATTR_GREEN_BRACELET = 16;
const TUint16 ATTR_YELLOW_BRACELET = 17;
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

  TBool IsWall(TFloat aWorldX, TFloat aWorldY) {
    return GetAttribute(aWorldX, aWorldY) == ATTR_WALL;
  }

  TBool IsFloor(TFloat aWorldX, TFloat aWorldY) {
    const TInt col = TInt(FLOOR(aWorldX / TILESIZE)),
              row = TInt(FLOOR(aWorldY / TILESIZE));
    if (row > 0 && row < mMapHeight - 1 && col > 0 && col < mMapWidth - 1) {
      const TInt attr = GetAttribute(aWorldX, aWorldY);
      return attr == ATTR_FLOOR || (attr == ATTR_LEDGE && (TInt(aWorldY) % 32 <= 8));
    }
    if (row <= 0 && aWorldY > -24) {
      return GetAttribute(0, col) == ATTR_FLOOR && GetAttribute(aWorldX, aWorldY + 24) == ATTR_FLOOR;
    }
    if (row >= mMapHeight - 1 && TInt(aWorldY) < mMapHeight * TILESIZE + 24) {
      return GetAttribute(mMapHeight - 1, col) == ATTR_FLOOR && GetAttribute(aWorldX, aWorldY - 24) == ATTR_FLOOR;
    }
    if (col <= 0 && aWorldX > -24) {
      return GetAttribute(row, 0) == ATTR_FLOOR && GetAttribute(aWorldX + 24, aWorldY) == ATTR_FLOOR;
    }
    if (col >= mMapWidth - 1 && TInt(aWorldX) < mMapWidth * TILESIZE + 24) {
      return GetAttribute(row, mMapWidth - 1) == ATTR_FLOOR && GetAttribute(aWorldX - 24, aWorldY) == ATTR_FLOOR;
    }

    return EFalse;
  }

  TBool IsLedge(TFloat aWorldX, TFloat aWorldY) {
    return GetAttribute(aWorldX, aWorldY) == ATTR_LEDGE && (TInt(aWorldY) % 32 >= 8);
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
};

#endif //MODITE_GGAMEPLAYFIELD_H
