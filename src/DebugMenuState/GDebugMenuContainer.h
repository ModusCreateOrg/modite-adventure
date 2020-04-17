#ifndef GENUS_GDEBUGMENUCONTAINER_H
#define GENUS_GDEBUGMENUCONTAINER_H

//#include "Game.h"
#include "common/GDialogWidget.h"

class BGameEngine;

enum {
  DEBUG_MAIN,
  DEBUG_LEVEL,
  DEBUG_INV,
};

class GDebugMenuContainer : public GDialogWidget {
public:
  GDebugMenuContainer(TInt aX, TInt aY, BGameEngine *mEngine);
  ~GDebugMenuContainer();
  TBool OnNavigate(TUint16 aBits);
  TInt Render(TInt aX, TInt aY);
  void AddWidget(BWidget &aWidget);
  void SetState(TInt aState);
  void Run();

public:
  TInt mTimer;
  TInt mState;
  TInt mSkip;
  TInt mDir;
  TInt mBuffer;
  TUint mLength;
  BGameEngine *mEngine;
};

#endif //GENUS_GDEBUGMENUCONTAINER_H
