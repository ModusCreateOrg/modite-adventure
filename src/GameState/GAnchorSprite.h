#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>
class GGameState;
//#include "GGameState.h"

enum DIRECTION {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
};

const TUint32 STYPE_OBJECT_BIT = STYPE_USER_BIT;
const TUint32 STYPE_OBJECT = 1 << STYPE_OBJECT_BIT;
const TUint32 STYPE_SPELL_BIT = STYPE_OBJECT_BIT + 1;
const TUint32 STYPE_SPELL = 1 << STYPE_SPELL_BIT;

enum HIT_STRENGTH {
  HIT_LIGHT,
  HIT_MEDIUM,
  HIT_HARD
};


// collisions occur only if two sprites' abs(delta y) is less than or equal to
// this:
const TFloat COLLISION_DELTA_Y = 16;

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);

  ~GAnchorSprite() OVERRIDE;

public:
  void Move() OVERRIDE;

  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void Collide(BSprite *aOther) OVERRIDE;

  void Nudge();

  TBool IsFloor(DIRECTION aDirection, TFloat aX, TFloat aY);

public:
  void Name(const char *aName) { strcpy(mName, aName); }

public:
  static DIRECTION RandomDirection();

  TBool CanWalk(DIRECTION aDireciton, TFloat aVx, TFloat aVy);

public:
  GGameState *mGameState;
  DIRECTION mDirection;
  GAnchorSprite *mCollided;
  TInt16 mLevel, mNextLevel, mExperience;
  TInt16 mHitPoints, mStrength, mDexterity;
  TInt mHitStrength;
  TInt mGold;
  TBool mInvulnerable;   // cannot be attacked
  TFloat mLastX, mLastY; // coordinates from last frame
  TFloat mVelocity;

protected:
  char mName[64];
};

#endif // MODITE_GANCHORSPRITE_H
