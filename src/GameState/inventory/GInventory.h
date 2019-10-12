#ifndef GINVENTORY_H
#define GINVENTORY_H

#include <BGameEngine.h>
class BViewPort;

class GInventoryPlayfield;

class GInventory : public BGameEngine {
public:
  EXPLICIT GInventory(BViewPort *aViewPort);
  ~GInventory() OVERRIDE;

public:
  void PostRender() OVERRIDE;

protected:
  GInventoryPlayfield *mPlayfield;
  TInt mTop, mCurrent;
};

#endif
