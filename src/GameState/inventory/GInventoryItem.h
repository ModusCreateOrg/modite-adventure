#ifndef GINVETORYITEM_H
#define GINVETORYITEM_H

class GInventoryList;

#include <BList.h>
#include <BMemoryStream.h>

struct GInventoryItem : public BNode {
public:
  GInventoryItem(TInt aItemNumber, GInventoryList *aInventoryList);
  ~GInventoryItem();

public:
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);

public:
  GInventoryList *mInventoryList;
  TInt16 mItemNumber;
  TInt16 mImage;
  TInt16 mCount;

  void Render();
};

#endif
