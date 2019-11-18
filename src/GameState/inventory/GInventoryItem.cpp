#include "GInventoryItem.h"
#include "GInventoryList.h"
#include "Items.h"

GInventoryItem::GInventoryItem(TInt aItemNumber, GInventoryList *aInventoryList) : BNode() {
  mItemNumber = aItemNumber;
  mInventoryList = aInventoryList;
  mImage = items[aItemNumber];
  mCount = 1;
  mInventoryList->mCount++;
}

GInventoryItem::~GInventoryItem() {
  mInventoryList->mCount--;
}

void GInventoryItem::Render() {
  printf("GInventoryItem: item: %d image: %d count: %d\n", mItemNumber, mImage, mCount);
}

void GInventoryItem::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mItemNumber, sizeof(mItemNumber));
  aStream.Write(&mImage, sizeof(mImage));
  aStream.Write(&mCount, sizeof(mCount));
}

void GInventoryItem::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mItemNumber, sizeof(mItemNumber));
  aStream.Read(&mImage, sizeof(mImage));
  aStream.Read(&mCount, sizeof(mCount));
}
