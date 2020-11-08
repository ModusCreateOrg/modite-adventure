#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

//#include "Game.h"
#include <BApplication.h>
#include "common/GSavedGameList.h"

class BGameEngine;
class BFont;

enum {
  GAME_STATE_SPLASH,
  GAME_STATE_MAIN_MENU,
  GAME_STATE_LOAD_GAME,
  GAME_STATE_MAIN_OPTIONS,
  GAME_STATE_RESET_OPTIONS,
  GAME_STATE_CREDITS,
  GAME_STATE_RESET_GAME,
  GAME_STATE_GAME,
  GAME_STATE_RESUME_GAME,
  GAME_STATE_LOAD_SAVEGAME,
  GAME_STATE_QUIT,
  GAME_STATE_VICTORY,
};

class GGame : public BApplication {
public:
  GGame();

  ~GGame() OVERRIDE;

public:
  void Run();

public:
  void SetState(TInt aNewState, TAny *aLocalData = ENull, TUint32 aSize = 0);
  void StartGame(char *aGameName);

  TInt GetState();

  void ToggleInGameMenu();
  void ToggleInventory();

#ifdef DEBUG_MODE
  void ToggleDebugMenu();
#endif
  TBool IsGameState();
  BGameEngine *CurrentState();

  static TBool mDebug;
protected:
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
