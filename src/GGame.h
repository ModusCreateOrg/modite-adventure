#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

#include "Game.h"
#include "common/GSavedGameList.h"

enum {
  GAME_STATE_SPLASH,
  GAME_STATE_MAIN_MENU,
  GAME_STATE_LOAD_GAME,
  GAME_STATE_MAIN_OPTIONS,
  GAME_STATE_RESET_OPTIONS,
  GAME_STATE_CREDITS,
  GAME_STATE_GAME,
  GAME_STATE_RESUME_GAME,
  GAME_STATE_QUIT,
};

class GGame : public BApplication {
public:
  GGame();

  virtual ~GGame();

public:
  void Run();

public:
  void SetState(TInt aNewState, TAny *aLocalData = ENull, TUint32 aSize = 0);
  void StartGame(char *aGameName);

  TInt GetState();

  void ToggleInGameMenu();
  void ToggleInventory();
  void ToggleDebugMenu();
  BGameEngine *CurrentState();

  static TBool mDebug;
protected:
  TBool IsGameState();
protected:
  TInt mState;
  TInt mNextState;
  TAny *mLocalData;   // arbitrary local data that is passed to SetState.
  TUint32 mLocalDataSize;
  BGameEngine *mGameMenu;
  BGameEngine *mInventory;
  BGameEngine *mDebugMenu;
  TRGB        mShmoo;
};

extern GGame *gGame;
extern BGameEngine *gGameEngine;
extern BFont *gFont8x8, *gFont16x16;

#endif //BRICKOUT_GGAME_H
