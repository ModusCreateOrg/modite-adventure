#ifndef MODITE_GGAMESTATE_H
#define MODITE_GGAMESTATE_H

#include "Game.h"

class GGamePlayfield;
class GPlayerProcess;

enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
} DIRECTION;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

  void PreRender();
protected:
  GGamePlayfield *mGamePlayfield;
  GPlayerProcess *mPlayerProcess;
};


#endif //MODITE_GGAMESTATE_H
