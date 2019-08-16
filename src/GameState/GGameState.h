#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#include "Game.h"

class GGamePlayfield;
class GPlayerProcess;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

  void PreRender();

  void LoadLevel(TUint16 aTileMapId);
protected:
  /**
   * Load bmp and remap it to the specified color range, and set the palette colors in gDisplay
   *
   * @param aBMP
   * @param aSlot
   * @param aPalette
   * @param aColors
   */
  void RemapSlot(TUint16 aBMP, TUint16 aSlot, TUint aPalette, TUint16 aColors);
protected:
  GGamePlayfield *mGamePlayfield;
  GPlayerProcess *mPlayerProcess;
};


#endif //MODITE_GGAMESTATE_H
