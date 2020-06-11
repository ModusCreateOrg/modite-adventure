#ifndef GENUS_GGAMEMENUCONTAINER_H
#define GENUS_GGAMEMENUCONTAINER_H

#include "Game.h"
#include "GGameState.h"
#include "common/GDialogWidget.h"

enum GAME_MENU_STATE {
  GAME_MENU_KEEP_STATE,
  GAME_MENU_PAUSE_STATE,
  GAME_MENU_OPTIONS_STATE,
};

class GGameMenuContainer : public GDialogWidget {
public:
  GGameMenuContainer(TInt aX, TInt aY, GGameState *aGameState);
  ~GGameMenuContainer() OVERRIDE;
  void PauseState();
  void OptionsState();
  void State(GAME_MENU_STATE aState);
  TInt Render(TInt aX, TInt aY) OVERRIDE;
  TInt mTimer;
  TInt8 mStateLevel;
protected:
  void Clear();
  GGameState *mGameState;
  GAME_MENU_STATE mState;
};

#endif //GENUS_GGAMEMENUCONTAINER_H
