#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

#include "Game.h"

enum {
  GAME_STATE_SPLASH,
  GAME_STATE_TITLE,
  GAME_STATE_GAME,
};

class GGame : public BApplication {
public:
  GGame();
  virtual ~GGame();
public:
  void Run();
public:
  void SetState(TInt aNewState);
protected:
  TInt mState;
  TInt mNextState;
};

extern GGame *gGame;

#endif //BRICKOUT_GGAME_H
