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
  ~GAnchorSprite() OVERRIDE;
public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;
  void Collide(BSprite *aOther) OVERRIDE;
public:
  DIRECTION mDirection;
  GAnchorSprite *mCollided;
  TInt16 mLevel;
  TInt16 mExperience;
  TInt16 mHitPoints;
  TInt16 mStrength;
  TInt16 mDexterity;
  TInt mGold;
};

#endif //MODITE_GANCHORSPRITE_H
