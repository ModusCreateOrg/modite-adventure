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
  GGamePlayfield *mGamePlayfield;
  GPlayerProcess *mPlayerProcess;
};


#endif //MODITE_GGAMESTATE_H
