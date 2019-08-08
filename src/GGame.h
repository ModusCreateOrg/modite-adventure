#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

#include "Game.h"

enum {
  GAME_STATE_SPLASH,
  GAME_STATE_MAIN_MENU,
  GAME_STATE_MAIN_OPTIONS,
  GAME_STATE_RESET_OPTIONS,
  GAME_STATE_CREDITS,
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
