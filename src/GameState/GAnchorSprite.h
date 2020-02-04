#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>
#include <BMemoryStream.h>

class GGameState;
//#include "GGameState.h"

enum DIRECTION {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
  DIRECTION_SPELL,
};

enum ELEMENT {
  ELEMENT_NONE,
  ELEMENT_WATER,
  ELEMENT_FIRE,
  ELEMENT_EARTH,
  ELEMENT_ENERGY,
};

const TUint32 STYPE_OBJECT_BIT = STYPE_USER_BIT;
const TUint32 STYPE_OBJECT = 1 << STYPE_OBJECT_BIT;
const TUint32 STYPE_SPELL_BIT = STYPE_OBJECT_BIT + 1;
const TUint32 STYPE_SPELL = 1 << STYPE_SPELL_BIT;

const TUint32 SFLAG_BELOW_BIT = SFLAG_USER_BIT;
const TUint32 SFLAG_BELOW = 1 << SFLAG_BELOW_BIT;
const TUint32 SFLAG_RENDER_SHADOW_BIT = SFLAG_USER_BIT + 1;
const TUint32 SFLAG_RENDER_SHADOW = 1 << SFLAG_RENDER_SHADOW_BIT; // sprite will be rendered with a shadow
const TUint32 SFLAG_KNOCKBACK_BIT = SFLAG_USER_BIT + 2;
const TUint32 SFLAG_KNOCKBACK = 1 << SFLAG_KNOCKBACK_BIT;

const TInt PRIORITY_BELOW = 500;

const TInt BASE_HIT_POINTS = 100;
const TInt BASE_STRENGTH = 25;
const TInt BASE_EXPERIENCE = 20;

static const TFloat SPELL_ATTACK_BONUS = 1.1;
static const TFloat RING_HIT_BONUS = 1.1;

struct GFloatRect {
  TFloat x1, y1, x2, y2;
  GFloatRect() {
    this->x1 = 0.0;
    this->y1 = 0.0;
    this->x2 = 0.0;
    this->y2 = 0.0;
  }
  void Set(TFloat aX1, TFloat aY1, TFloat aX2, TFloat aY2) {
    this->x1 = aX1;
    this->y1 = aY1;
    this->x2 = aX2;
    this->y2 = aY2;
  }
  void Offset(TFloat aDX, TFloat aDY) {
    x1 += aDX;
    y1 += aDY;
    x2 += aDX;
    y2 += aDY;
  }
};

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

  TBool IsFloorTile(TFloat aX, TFloat aY);

  TBool IsFloor(DIRECTION aDirection, TFloat aX, TFloat aY);

  void SafePosition(BSprite *aOther);

  void SetLevel(TInt aLevel);

  void SetStatMultipliers(TFloat aModHitPoints = 1.0, TFloat aModStrength = 1.0, TFloat aModExperience = 1.0);

  void ResetShadow();

  void GetFloatRect(GFloatRect &aRect);

public:
  void Name(const char *aName) { strcpy(mName, aName); }
  const char *Name() { return mName; }

public:
  void Dump();

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
  TInt16 mHitPoints, mMaxHitPoints;
  TInt32 mAttackStrength;
  TInt32 mBaseHitPoints, mBaseStrength, mBaseExperience;
  TBool mInvulnerable;   // cannot be attacked
  TFloat mLastX, mLastY; // coordinates from last frame
  TFloat mVelocity;
  TRect mShadow;
  TUint mAttributeSave;
  ELEMENT mElement;

public:
  void WriteToStream(BMemoryStream &aStream);
  void WriteCustomToStream(BMemoryStream &aStream);
  virtual void ReadFromStream(BMemoryStream &aStream);
  virtual void ReadCustomFromStream(BMemoryStream &aStream) {}

protected:
  char mName[64];
};

#endif // MODITE_GANCHORSPRITE_H
