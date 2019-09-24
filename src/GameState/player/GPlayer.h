/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

#include "Game.h"
#include "GItemProcess.h"
#include "GPlayerProcess.h"
#include "GPlayer.h"

struct GInventoryItem : public BNode {
  GInventoryItem(TInt aItemNumber);

  TInt mItemNumber;
  TInt mImage;
  TInt mCount;
  void Render();
};

struct GInventoryList : public BList {
  GInventoryList();

  void AddItem(GInventoryItem *aItem) {
    AddHead(*aItem);
  }

  void RemoveItem(GInventoryItem *aItem) {
    aItem->Remove();
  }

  void DropItem(GInventoryItem *aItem);

  void PickupItem(TInt aItemNumber);

  GInventoryItem *First() {
    return (GInventoryItem *)BList::First();
  }
  TBool End(GInventoryItem *item) {
    return BList::End((BNode *)item);
  }
  GInventoryItem *Next(GInventoryItem *item) {
    return (GInventoryItem *)BList::Next((BNode *)item);
  }

  void Dump();
};


struct GPlayer {
  GPlayer() {
    printf("Construct GPlayer\n");
    mLevel       = 1;
    mNextLevel   = 100;
    mExperience  = 0;
    mHitPoints   = 10;
    mStrength    = 10;
    mDexterity   = 10;
    mHitStrength = 1;
    mGold        = 0;
  }

  static TInt16         mLevel, mNextLevel, mExperience;
  static TInt16         mHitPoints, mStrength, mDexterity;
  static TInt           mHitStrength;
  static TInt           mGold;
  static GInventoryList mInventoryList;
  static GPlayerProcess *mProcess;
  static GAnchorSprite  *mSprite;
  static GGameState     *mGameState;
};

#endif //MODITE_GPLAYER_H
