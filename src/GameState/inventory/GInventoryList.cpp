#include "GInventoryList.h"
#include "GItemProcess.h"
#include "GPlayer.h"
#include "Items.h"

GInventoryList::GInventoryList() : BList() {
  Reset();
  mCount = 0;
  //
}

void GInventoryList::DropItem(GInventoryItem *aItem) {
  if (--aItem->mCount <= 0) {
    aItem->Remove();
    delete aItem;
  }
  GItemProcess::SpawnItem(GPlayer::mGameState, -1, aItem->mItemNumber, GPlayer::mSprite->x, GPlayer::mSprite->y);
}

void GInventoryList::PickupItem(TInt aItemNumber) {
  // item already in inventory?  If so, increment count
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    if (i->mItemNumber == aItemNumber) {
      i->mCount++;
      return;
    }
  }
  AddItem(new GInventoryItem(aItemNumber, this));
}

GInventoryItem *GInventoryList::FindItem(TInt16 aItemNumber) {
  // item already in inventory?  If so, increment count
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    if (i->mItemNumber == aItemNumber) {
      return i;
    }
  }
  return ENull;
}

TBool GInventoryList::UseItem(TInt16 aItemNumber) {
  GInventoryItem *item = FindItem(aItemNumber);
  if (!item) {
    return EFalse;
  }
  switch (aItemNumber) {
    case ITEM_SILVER_KEY:
    case ITEM_GOLD_KEY:
    case ITEM_RED_POTION1:
    case ITEM_RED_POTION2:
    case ITEM_BLUE_POTION1:
    case ITEM_BLUE_POTION2:
    case ITEM_EMPTY_BOTTLE:
    case ITEM_BLUE_BOTTLE1:
    case ITEM_BLUE_BOTTLE2:
    case ITEM_BLUE_BOTTLE3:
    case ITEM_BLUE_BOTTLE4:
    case ITEM_RED_BOTTLE1:
    case ITEM_RED_BOTTLE2:
    case ITEM_RED_BOTTLE3:
    case ITEM_RED_BOTTLE4:
      if (item->mCount < 1) {
        return EFalse;
      }
      item->mCount--;
      if (item->mCount == 0) {
        item->Remove();
        delete item;
      }
    default:
      return ETrue;
  }
}

void GInventoryList::Dump() {
  // print inventory list to console
  printf("\n\nInventory (%d items)\n", mCount);
  printf("=========\n");
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    printf(" item #: %3d %-32s img: %3d count: %3d\n", i->mItemNumber, itemNames[i->mItemNumber], i->mImage, i->mCount);
  }
}

void GInventoryList::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mCount, sizeof(mCount));
  for (GInventoryItem *i = First(); !End(i); i = Next(i)) {
    i->WriteToStream(aStream);
  }
}

void GInventoryList::ReadFromStream(BMemoryStream &aStream) {

  TInt16 count;
  aStream.Read(&count, sizeof(count));
  for (TInt n = 0; n < count; n++) {
    GInventoryItem *i = new GInventoryItem(0, this);
    i->ReadFromStream(aStream);
    AddItem(i);
  }
}
