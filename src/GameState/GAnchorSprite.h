#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>

enum DIRECTION {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
};

enum HIT_STRENGTH { HIT_LIGHT, HIT_MEDIUM, HIT_HARD };

// collisions occur only if two sprites' abs(delta y) is less than or equal to
// this:
const TFloat COLLISION_DELTA_Y = 16;

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(
      TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);

  ~GAnchorSprite() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void Collide(BSprite *aOther) OVERRIDE;

  void Nudge();

public:
  void Name(const char *aName) { strcpy(mName, aName); }

public:
  DIRECTION mDirection;
  GAnchorSprite *mCollided;
  TInt16 mLevel, mNextLevel, mExperience;
  TInt16 mHitPoints, mStrength, mDexterity;
  TInt mHitStrength;
  TInt mGold;
  char mName[64];
  TBool mInvulnerable; // cannot be attacked
};

#endif // MODITE_GANCHORSPRITE_H
