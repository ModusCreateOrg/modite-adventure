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
  GGamePlayfield *mGamePlayfield;
  GPlayerProcess *mPlayerProcess;
};


#endif //MODITE_GGAMESTATE_H
