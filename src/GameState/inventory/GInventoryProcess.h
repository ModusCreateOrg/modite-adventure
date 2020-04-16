#ifndef MODITE_GINVENTORYPROCESS_H
#define MODITE_GINVENTORYPROCESS_H

#include "GGameState.h"
#include "GInventoryItem.h"
#include "GPlayerSprite.h"

class GInventory;
class GInventoryProcess : public BProcess {
public:
  GInventoryProcess(GInventory *aGameState);
  ~GInventoryProcess();

  TBool RunBefore();
  TBool RunAfter();

protected:
    GInventory *mGameState;
    GPlayerSprite *mSprite;

public:
    GInventoryItem *mSelectedItem;
    TInt16 mItemNumber;
    TInt mDebounce;
    TInt mCurrentRow, mCurrentColumn;
};

#endif //MODITE_GINVENTORYPROCESS_H
