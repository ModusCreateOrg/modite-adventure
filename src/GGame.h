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

  TInt GetState();

  void ToggleInGameMenu();

#ifdef DEBUG_MODE
  static TBool mDebug;
#endif

protected:
  TInt        mState;
  TInt        mNextState;
  BGameEngine *mGameMenu;
  TRGB        mShmoo;
};

extern GGame *gGame;
extern BGameEngine *gGameEngine;
extern BFont *gFont8x8, *gFont16x16;

#endif //BRICKOUT_GGAME_H
