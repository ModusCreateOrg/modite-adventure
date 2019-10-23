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

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

  void GameLoop() OVERRIDE;
  void PreRender();
  void PostRender();

  void NextLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId);
  void LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId);

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
  GGamePlayfield *mGamePlayfield;

protected:
  TInt mTimer;
  char mText[128];
  char mName[128];
  TInt16 mNextLevel, mLevel;
  TUint16 mNextTileMapId, mTileMapId;

public:
  GAnchorSprite *PlayerSprite();
};

#endif //MODITE_GGAMESTATE_H
