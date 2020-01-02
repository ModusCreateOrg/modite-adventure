#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#define TILES_WIDE 10
#define TILES_HIGH 7

#define ENABLE_REMAP_SLOTS
//#undef ENABLE_REMAP_SLOTS

#include "Game.h"
#include "GGamePlayfield.h"

class GAnchorSprite;

class GGamePlayfield;

// Message Types
static TUint16 MESSAGE_FLOOR_SWITCH_DOWN = 0;
static TUint16 MESSAGE_FLOOR_SWITCH_UP = MESSAGE_FLOOR_SWITCH_DOWN + 1;

const TInt16 OVERWORLD_DUNGEON = 0;
const TInt16 DUNGEON_257 = 1;
const TInt16 DUNGEON_258 = 1;
const TInt16 DUNGEON_259 = 1;

#include "GPlayerProcess.h"

struct TDungeonInfo {
  const char *name;
  struct {
    TInt16 objectsId;
    TInt16 map[11];
  } mInfo;
};
extern TDungeonInfo gDungeonDefs[];
extern const TInt NUM_DUNGEONS;

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

  void SetPlayfieldXYFromPlayer(TFloat aPlayerX, TFloat aPlayerY);

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

public:
  GGamePlayfield *mGamePlayfield, *mNextGamePlayfield;

public:
  TBool IsGameOver() { return mGameOver != ENull; }

  TInt16 IsCurrentLevel(TUint16 aDungeon, TInt16 aLevel) { return mNextDungeon == aDungeon && mLevel == aLevel; }
  TInt16 LastOverworldLevel() { return mLastOverworldLevel; }
protected:
  TInt mTimer;
  char mText[128];
  char mName[128];
  TInt16 mNextLevel, mLevel, mNextObjectsId, mLastOverworldLevel;
  TUint16 mDungeon, mNextDungeon, mNextTileMapId, mTileMapId;
  TUint16 mPlayerToLoad;
  GGameOver *mGameOver;
#ifdef ENABLE_REMAP_SLOTS
protected:
  // If the value for the slot is ETrue, then the slot
  // has already been remapped. We don't want to remap twice or the color range
  // ends up in the wrong place.
  TBool mSlotRemapState[SLOT_MAX];
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

  void InitRemapSlots();
#endif

};

#endif //MODITE_GGAMESTATE_H
