#ifndef GINVENTORYLIST_H
#define GINVENTORYLIST_H

#include <BList.h>
#include <BMemoryStream.h>
#include "GInventoryItem.h"

class GInventoryList;

struct GInventoryList : public BList {
  GInventoryList();
  ~GInventoryList() {
    Reset();
  }

  void AddItem(GInventoryItem *aItem) {
    AddHead(*aItem);
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

  GInventoryItem *FindItem(TInt16 aItemNumber);

  TBool UseItem(TInt16 aItemNumber);

public:
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);

public:
  void Dump();
  void FullReset();

public:
  TUint16 mCount; // number of items in list
};

#endif
