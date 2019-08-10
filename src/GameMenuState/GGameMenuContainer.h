#ifndef GENUS_GGAMEMENUCONTAINER_H
#define GENUS_GGAMEMENUCONTAINER_H

#include "Game.h"

class GGameMenuContainer : public GDialogWidget {
public:
  GGameMenuContainer(TInt aX, TInt aY);
  ~GGameMenuContainer();
  TInt Render(TInt aX, TInt aY);
  TInt mTimer;
};

#endif //GENUS_GGAMEMENUCONTAINER_H
