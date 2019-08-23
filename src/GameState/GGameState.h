#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#define TILES_WIDE 10
#define TILES_HIGH 7

#include "Game.h"

class GGamePlayfield;
class GPlayerProcess;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

  void PreRender();

  void LoadLevel(TUint16 aTileMapId);

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

public:
  GAnchorSprite *PlayerSprite() {
    return mPlayerProcess->mSprite;
  }
};


#endif //MODITE_GGAMESTATE_H
