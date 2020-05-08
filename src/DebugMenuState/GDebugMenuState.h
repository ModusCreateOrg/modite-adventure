#ifndef GENUS_GDEBUGMENUSTATE_H
#define GENUS_GDEBUGMENUSTATE_H

//#include "Game.h"
#include "GDebugMenuContainer.h"
#include "BGameEngine.h"

class GDebugMenuContainer;

class GDebugMenuState : public BGameEngine {
public:
  GDebugMenuState();
  virtual ~GDebugMenuState();
  void PreRender();
  void PostRender();

private:
  GDebugMenuContainer *mContainer;
  TRGB mSavedPalette[256];

};

#endif //GENUS_GDEBUGMENUSTATE_H
