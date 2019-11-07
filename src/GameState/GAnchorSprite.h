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

const TUint32 SFLAG_BELOW_BIT = SFLAG_USER_BIT;
const TUint32 SFLAG_BELOW = 1 << SFLAG_BELOW_BIT;
const TUint32 SFLAG_COLLIDE2D_BIT = SFLAG_BELOW_BIT+1;
const TUint32 SFLAG_COLLIDE2D = 1 << SFLAG_COLLIDE2D_BIT;

const TInt PRIORITY_BELOW = 500;
// collisions occur only if two sprites' abs(delta y) is less than or equal to
// this:
const TFloat DEFAULT_COLLISION_DELTA_Y = 6;

// Sprite box is adjusted by these (smaller) to make walking (avoid wall collision) more generous
const TFloat FLOOR_ADJUST_LEFT = 4;
const TFloat FLOOR_ADJUST_TOP = 2;
const TFloat FLOOR_ADJUST_RIGHT = 4;
const TFloat FLOOR_ADJUST_BOTTOM = 2;

const TInt BASE_HIT_POINTS = 100;
const TInt BASE_STRENGTH = 25;
const TInt BASE_EXPERIENCE = 20;

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);

  ~GAnchorSprite() OVERRIDE;

public:
  void Move() OVERRIDE;

  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void Collide(BSprite *aOther) OVERRIDE;

  void Nudge();

  void SetWall(TBool aState = ETrue);

  TBool IsFloorTile(GAnchorSprite *aSprite, TFloat aX, TFloat aY);

  TBool IsFloor(DIRECTION aDirection, TFloat aX, TFloat aY);

  void SafePosition(BSprite *aOther);

  void SetLevel(TInt aLevel);

  void SetStatMultipliers(TFloat aModHitPoints = 1.0, TFloat aModStrength = 1.0, TFloat aModExperience = 1.0);

public:
  void Name(const char *aName) { strcpy(mName, aName); }
  const char *Name() { return mName; }

public:
  static DIRECTION RandomDirection();

  TBool CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy);
  // set the BMapPlayfield tile in map attribute
  void SetAttribute(TUint mAttribute);
public:
  GGameState *mGameState;
  DIRECTION mDirection;
  GAnchorSprite *mCollided;
  TInt16 mLevel, mExperience;
  TInt16 mHitPoints;
  TInt mHitStrength;
  TInt mBaseHitPoints, mBaseStrength, mBaseExperience;
  TBool mInvulnerable;   // cannot be attacked
  TInt16 mCollisionDeltaY;
  TFloat mLastX, mLastY; // coordinates from last frame
  TFloat mVelocity;
  TUint mAttributeSave;

protected:
  char mName[64];
};

#endif // MODITE_GANCHORSPRITE_H
