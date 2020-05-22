#ifndef MODITE_GINVENTORYPROCESS_H
#define MODITE_GINVENTORYPROCESS_H

#include "GGameState.h"
#include "GInventoryItem.h"
#include "GPlayerSprite.h"

class GInventory;
class GInventoryProcess : public BProcess {
public:
  EXPLICIT GInventoryProcess(GInventory *aGameState);
  ~GInventoryProcess() OVERRIDE;

  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

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
