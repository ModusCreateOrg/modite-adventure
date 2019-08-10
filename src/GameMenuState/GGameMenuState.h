#ifndef GENUS_GGAMEMENUSTATE_H
#define GENUS_GGAMEMENUSTATE_H

#include "Game.h"

class GGameMenuContainer;

class GGameMenuState : public BGameEngine {
public:
  GGameMenuState();
  virtual ~GGameMenuState();
  void PreRender();
  void PostRender();

private:
  BFont *mFont16;
  GGameMenuContainer *mContainer;
};

#endif //GENUS_GGAMEMENUSTATE_H
