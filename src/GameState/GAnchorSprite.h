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

const TFloat SPELLBOOK_MATRIX[4][4] = {
     // water,   fire,  earth, energy
        { 1.0,    1.0,    1.0,    1.0 }, // water
        { 1.5,    1.0,    1.5,    1.5 }, // fire
        { 1.0,    1.5,    1.0,    1.5 }, // earth
        { 1.5,    1.5,    1.5,    1.0 }, // energy
};

const TFloat RING_MATRIX[4][4] = {
      // water,   fire,  earth, energy
        { 1.0 ,   1.0 ,   1.0 ,   1.0  }, // water
        { 1.25,   1.0 ,   1.25,   1.25 }, // fire
        { 1.0 ,   1.25,   1.0 ,   1.25 }, // earth
        { 1.25,   1.25,   1.25,   1.0  }, // energy
};

const TFloat AMULET_MATRIX[4][4] = {
      // water,   fire,  earth, energy
        { 0.25,   2.0 ,   2.0 ,   2.0  }, // water
        { 1.0 ,   0.25,   1.0 ,   1.0  }, // fire
        { 1.0 ,   1.0 ,   0.25,   1.0  }, // earth
        { 2.0 ,   1.0 ,   1.0 ,   0.25 }, // energy
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

// Sprite box is adjusted by these (smaller) to make walking (avoid wall collision) more generous
const TFloat FLOOR_ADJUST_LEFT = 1;
const TFloat FLOOR_ADJUST_TOP = 1;
const TFloat FLOOR_ADJUST_RIGHT = 1;
const TFloat FLOOR_ADJUST_BOTTOM = 1;

const TInt BASE_HIT_POINTS = 100;
const TInt BASE_STRENGTH = 25;
const TInt BASE_EXPERIENCE = 20;

static const TFloat SPELL_HIT_BONUS = 1.1;
static const TFloat RING_HIT_BONUS = 1.1;

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

  TBool MaybeDamage(TBool aSpell);

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
  TInt32 mHitStrength;
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
