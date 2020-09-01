#ifndef MODITE_GANCHORSPRITE_H
#define MODITE_GANCHORSPRITE_H

#include <BAnimSprite.h>
#include <BMemoryStream.h>
#include "Game.h"

class GGameState;
//#include "GGameState.h"

enum DIRECTION {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
  DIRECTION_UNSPECIFIED,
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
const TUint32 STYPE_SPELL_BIT = STYPE_USER_BIT + 1;
const TUint32 STYPE_SPELL = 1 << STYPE_SPELL_BIT;

const TUint32 SFLAG_BELOW_BIT = SFLAG_USER_BIT;
const TUint32 SFLAG_BELOW = 1 << SFLAG_BELOW_BIT;
const TUint32 SFLAG_RENDER_SHADOW_BIT = SFLAG_USER_BIT + 1;
const TUint32 SFLAG_RENDER_SHADOW = 1 << SFLAG_RENDER_SHADOW_BIT; // sprite will be rendered with a shadow
const TUint32 SFLAG_KNOCKBACK_BIT = SFLAG_USER_BIT + 2;
const TUint32 SFLAG_KNOCKBACK = 1 << SFLAG_KNOCKBACK_BIT;

const TUint32 SFLAG_RENDER_DEBUG_BIT = SFLAG_USER_BIT + 3;
const TUint32 SFLAG_RENDER_DEBUG = 1 << SFLAG_RENDER_DEBUG_BIT;
//const TUint32 SFLAG_RENDER_DEBUG = 0;
///asdfsafd
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

struct GCollidedData {
  DIRECTION direction = DIRECTION_DOWN;
  ELEMENT element = ELEMENT_NONE;
  TInt32 attackStrength = 0;
  TFloat collisionAngle = 0.0;
  TFloat vx = 0.0, vy = 0.0;
  TUint32 flags = 0;
};

class GAnchorSprite : public BAnimSprite {
public:
  GAnchorSprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);

  ~GAnchorSprite() OVERRIDE;

public:
  void Move() OVERRIDE;

  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void SaveDataFromCollided(GAnchorSprite *aOther);
  void Collide(BSprite *aOther) OVERRIDE;

  void Nudge();

  TBool IsFloorTile(TFloat aX, TFloat aY);

  TBool CanWalkInDirection(DIRECTION aDirection, TFloat aDx = 0.0, TFloat aDy = 0.0);
  TBool CanWalk(TFloat aDx, TFloat aDy, TBool aCheckAllSides = EFalse);

  void SafePosition(BSprite *aOther);

  void ResetShadow();

  void GetFloatRect(GFloatRect &aRect);

  TPoint Center();

public:
  void Name(const char *aName) { strcpy(mName, aName); }
  const char *Name() { return mName; }

public:
  void Dump();

public:
  static DIRECTION RandomDirection();
  static DIRECTION RotateDirection(DIRECTION aDirection, TInt aRotateClockwiseCount);

  void StartAnimationInDirection(ANIMSCRIPT* aScriptGroup[4], DIRECTION aDirection);
  void MoveInDirection(TFloat aSpeed, DIRECTION aDirection, TBool aIgnoreWallFlag = EFalse);

  // set the BMapPlayfield tile in map attribute
  void SetMapAttribute(TUint aAttribute);
  void ResetMapAttribute();

public:
  GGameState *mGameState;
  DIRECTION mDirection;
  GCollidedData mCollided;
  TInt32 mAttackStrength;
  TFloat mLastX, mLastY; // coordinates from last frame
  TRect mShadow;
  TUint mAttributeSave;
  ELEMENT mElement;

public:
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);
  virtual void WriteCustomToStream(BMemoryStream &aStream) {}
  virtual void ReadCustomFromStream(BMemoryStream &aStream) {}

protected:
  char mName[64];
};

#endif // MODITE_GANCHORSPRITE_H
