#ifndef GENUS_GGAMEMENUSTATE_H
#define GENUS_GGAMEMENUSTATE_H

//#include "Game.h"

class GGameMenuContainer;
class GGameState;

class GGameMenuState : public BGameEngine {
public:
  GGameMenuState(GGameState *aGameState);
  virtual ~GGameMenuState();
  void PreRender();
  void PostRender();

private:
  GGameState *mGameState;
  BFont *mFont16;
  GGameMenuContainer *mContainer;
  TRGB mSavedPalette[256];
};

#endif //GENUS_GGAMEMENUSTATE_H
