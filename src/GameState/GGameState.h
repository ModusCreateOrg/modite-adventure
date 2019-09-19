#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#define TILES_WIDE 10
#define TILES_HIGH 7

#include "Game.h"

class GGamePlayfield;
class GPlayerProcess;
class GAnchorSprite;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

  void PreRender();
  void PostRender();

  void NextLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId);
  void LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId);

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
  void RemapSlot(TUint16 aBMP, TUint16 aSlot);

protected:
  GGamePlayfield *mGamePlayfield;
  GPlayerProcess *mPlayerProcess;
  TInt mTimer;
  TBool mStats;
  char mText[128];
  char mName[128];
  TInt16 mNextLevel, mLevel;
  TUint16 mNextTileMapId, mTileMapId;

public:
  GAnchorSprite *PlayerSprite() ;
};

#endif //MODITE_GGAMESTATE_H
