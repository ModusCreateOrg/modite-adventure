#include "GPlayer.h"
#include "Items.h"

GInventoryList::GInventoryList() : BList() {
  Reset();
  //
}

void GInventoryList::DropItem(GInventoryItem *aItem) {
  if (--aItem->mCount <= 0) {
    aItem->Remove();
  }
  GItemProcess::SpawnItem(GPlayer::mGameState, aItem->mItemNumber, GPlayer::mSprite->x, GPlayer::mSprite->y);
}

void GInventoryList::PickupItem(TInt aItemNumber) {
  // item already in inventory?  If so, increment count
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    if (i->mItemNumber == aItemNumber) {
      i->mCount++;
      return;
    }
  }
  AddItem(new GInventoryItem(aItemNumber));
}

void GInventoryList::Dump() {
  // print inventory list to console
  printf("\n\nInventory\n");
  printf("=========\n");
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    printf(" item #: %3d %-32s img: %3d count: %3d\n", i->mItemNumber, itemNames[i->mItemNumber], i->mImage, i->mCount);
  }
}
GInventoryItem::GInventoryItem(TInt aItemNumber) : BNode() {
  mItemNumber = aItemNumber;
  mImage = items[aItemNumber];
  mCount = 1;
}

void GInventoryItem::Render() {
  printf("GInventoryItem: item: %d image: %d count: %d\n", mItemNumber, mImage, mCount);
}

TInt16 GPlayer::mLevel;
TInt16 GPlayer::mNextLevel;
TInt16 GPlayer::mExperience;
TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;
TInt16 GPlayer::mStrength;
TInt16 GPlayer::mDexterity;

TInt GPlayer::mHitStrength;
TInt GPlayer::mGold;
TInt GPlayer::mHealthPotion;
TInt GPlayer::mManaPotion;

GInventoryList GPlayer::mInventoryList;
GPlayerProcess *GPlayer::mProcess;
GAnchorSprite *GPlayer::mSprite;
GGameState *GPlayer::mGameState;
GEquipped GPlayer::mEquipped;
