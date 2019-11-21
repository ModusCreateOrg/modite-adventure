#ifndef GENUS_GDEBUGMENUCONTAINER_H
#define GENUS_GDEBUGMENUCONTAINER_H

#include "Game.h"

class GDebugMenuContainer : public GDialogWidget {
public:
  GDebugMenuContainer(TInt aX, TInt aY, BGameEngine *aState);
  ~GDebugMenuContainer();
  TBool OnNavigate(TUint16 aBits);
  TInt Render(TInt aX, TInt aY);
  void AddWidget(BWidget &aWidget);

public:
  TInt mTimer;

protected:
  TUint mLength = 0;
  TInt mSkip = 0;
  TInt mDir = 0;
  TInt mBuffer = 0;
};

#endif //GENUS_GDEBUGMENUCONTAINER_H
