#ifndef GENUS_GGAMEMENUCONTAINER_H
#define GENUS_GGAMEMENUCONTAINER_H

#include "Game.h"

class GGameMenuContainer : public GDialogWidget {
public:
  GGameMenuContainer(TInt aX, TInt aY, GGameState *aGameState);
  ~GGameMenuContainer();
  TInt Render(TInt aX, TInt aY);
  TInt mTimer;
protected:
  GGameState *mGameState;
};

#endif //GENUS_GGAMEMENUCONTAINER_H
