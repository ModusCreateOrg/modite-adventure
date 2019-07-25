#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);
  virtual ~GAnchorSprite();
public:
  TBool Render(BViewPort *aViewPort);
};


#endif //MODITE_GANCHORSPRITE_H
