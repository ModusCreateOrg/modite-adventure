#ifndef GENUS_GDEBUGMENUCONTAINER_H
#define GENUS_GDEBUGMENUCONTAINER_H

#include "Game.h"

class GDebugMenuContainer : public GDialogWidget {
public:
  GDebugMenuContainer(TInt aX, TInt aY);
  ~GDebugMenuContainer();
  TInt Render(TInt aX, TInt aY);
  TInt mTimer;
};

#endif //GENUS_GDEBUGMENUCONTAINER_H
