#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>

enum DIRECTION {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
};

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);
  virtual ~GAnchorSprite();
public:
  TBool Render(BViewPort *aViewPort);
  void Collide(BSprite *aOther);
public:
  DIRECTION mDirection;
  GAnchorSprite *mCollided;
  TInt mHitPoints;
};

#endif //MODITE_GANCHORSPRITE_H
