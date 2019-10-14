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
  void RenderInventory();
  void RenderEquipped();

protected:
  GInventoryPlayfield *mPlayfield;
  TInt mTop, mCurrent;
  TInt mMode;
};

#endif
