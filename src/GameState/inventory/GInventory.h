#ifndef GINVENTORY_H
#define GINVENTORY_H

#include <BGameEngine.h>
#include "GInventoryItem.h"

class BViewPort;
class GInventoryProcess;

class GInventory : public BGameEngine {
public:
  EXPLICIT GInventory(BViewPort *aViewPort);
  ~GInventory() OVERRIDE;

public:
  void PostRender() OVERRIDE;
  GInventoryItem *SelectedItem();
  TInt16 ItemNumber();
  TInt16 ItemLayout(TUint8 x, TUint8 y);
  const TUint16 *ItemLayout(TUint8 x);
  TInt CurrentRow();
  TInt CurrentColumn();

protected:
  GInventoryProcess *mProcess;
};

#endif
