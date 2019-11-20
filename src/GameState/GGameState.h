#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#define TILES_WIDE 10
#define TILES_HIGH 7

#include "Game.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"

class GPlayerProcess;
class GAnchorSprite;
class GGamePlayfield;

// Message Types
static TUint16 MESSAGE_FLOOR_SWITCH_DOWN = 0;
static TUint16 MESSAGE_FLOOR_SWITCH_UP = MESSAGE_FLOOR_SWITCH_DOWN + 1;

const TInt16 DUNGEON_DEV = 0;
const TInt16 DUNGEON_LIVE = 1;

// info about the dungeons
static struct DUNGEON_DEF {
  const char *name;
  TUint16 map[10];
} dungeon_defs[] = {
//   DUNGEON_DEV
  { "DEV DUNGEON",
      {
          DEVDUNGEON_0_LEVEL_1_MAP,
          DEVDUNGEON_0_LEVEL_1_MAP,
          DEVDUNGEON_0_LEVEL_2_MAP,
          DEVDUNGEON_0_LEVEL_3_MAP,
          DEVDUNGEON_0_LEVEL_4_MAP,
      } },
};
const TInt NUM_DUNGEONS = sizeof(dungeon_defs) / sizeof(DUNGEON_DEF);


class GGameOver;

class GGameState : public BGameEngine {
public:
  // game engine
  GGameState();
  // game engine loads saved game
  GGameState(const char *aName);

  ~GGameState();

  void Init();

public:
  void GameLoop() OVERRIDE;
  void PreRender() OVERRIDE;
  void PostRender() OVERRIDE;

  void NextLevel(const TInt16 aDungeon, const TInt16 aLevel);
  void LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId, TBool aNewLevel = ETrue);

  TBool SaveState();
  TBool LoadState(const char *aGameName);

public:
  GProcess *AddProcess(GProcess *p);
  // call this to resume game after death
  void TryAgain();

  // call this when player is dead
  void GameOver();

  // store code and attr into ObjectProgram at aIp
  void EndProgram(TInt aIp, TUint16 aCode = ATTR_GONE, TUint16 aAttr = 0);

  TUint16 MapWidth();
  TUint16 MapHeight();

protected:
  /**
   * Load bmp and remap it to the specified color range, and set the palette colors in gDisplay
   *
   * @param aBMP
   * @param aSlot
   * @param aPalette
   * @param aColors
   */
  void RemapSlot(TUint16 aBMP, TUint16 aSlot, TInt16 aImageSize = IMAGE_64x64);

public:
  GGamePlayfield *mGamePlayfield, *mPreviousPlayfield;

public:
  TBool IsGameOver() { return mGameOver != ENull; }

protected:
  TInt mTimer;
  char mText[128];
  char mName[128];
  TInt16 mNextLevel, mLevel;
  TUint16 mNextDungeon, mNextTileMapId, mTileMapId;
  GGameOver *mGameOver;

public:
  GAnchorSprite *PlayerSprite();
};

#endif //MODITE_GGAMESTATE_H
