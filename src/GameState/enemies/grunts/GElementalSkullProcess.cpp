#include "GElementalSkullProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;
const TInt IDLE_SPEED = 6 * FACTOR;
const TUint8 BLINK_TIME = 4 * FACTOR;
const TUint8 OSCILLATE_MAX_X = 13;
const TUint8 OSCILLATE_MAX_Y = 4;

const TBool DAMAGE_MATRIX[4][4] = {
  // water, fire, earth, energy
  { 0, 1, 0, 0 }, // water
  { 1, 0, 0, 0 }, // fire
  { 0, 0, 0, 1  }, // earth
  { 0, 0, 1, 0 }, // energy
};

TUint8 GElementalSkullProcess::mInstances = 0;

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */

ANIMSCRIPT idleAnimationEarth[] = {
  ABITMAP(ELEMENTAL_SKULL_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, 0),
  ASTEP(IDLE_SPEED, 1),
  ASTEP(IDLE_SPEED, 2),
  ALOOP
};

ANIMSCRIPT idleAnimationWater[] = {
  ABITMAP(ELEMENTAL_SKULL_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, 3),
  ASTEP(IDLE_SPEED, 4),
  ASTEP(IDLE_SPEED, 5),
  ALOOP
};

ANIMSCRIPT idleAnimationFire[] = {
  ABITMAP(ELEMENTAL_SKULL_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, 6),
  ASTEP(IDLE_SPEED, 7),
  ASTEP(IDLE_SPEED, 8),
  ALOOP
};

ANIMSCRIPT idleAnimationEnergy[] = {
  ABITMAP(ELEMENTAL_SKULL_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, 9),
  ASTEP(IDLE_SPEED, 10),
  ASTEP(IDLE_SPEED, 11),
  ALOOP
};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GElementalSkullProcess::GElementalSkullProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam, TUint16 aAttribute)
    : GGruntProcess(aGameState, aIp, aX, aY, ELEMENTAL_SKULL_SLOT, aParam, 1, aAttribute) {
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cx = 0;
  mSprite->cy = 4;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_ELEMENTAL_SKULLS_BMP_SPRITES);
  mSprite->SetFlags(SFLAG_KILL_ON_IMPACT);
  GElementalSkullProcess::mInstances++;

  mTaunt = EFalse;
  mHitPoints = 1;
  mMaxHitPoints = 1;
  mExperienceYield = 0;
  mTimer = OSCILLATE_MAX_X;

  switch (aAttribute) {
    case ATTR_ELEMENTAL_SKULL_EARTH:
      mSprite->Name("ENEMY EARTH ELEMENTAL SKULL");
      mSprite->mElement = ELEMENT_EARTH;
      mSprite->vx = .7;
      mSprite->vy = -.15;
      mAnimation = idleAnimationEarth;
      break;
    case ATTR_ELEMENTAL_SKULL_WATER:
      mSprite->Name("ENEMY WATER ELEMENTAL SKULL");
      mSprite->mElement = ELEMENT_WATER;
      mSprite->vx = -.95;
      mSprite->vy = -.25;
      mAnimation = idleAnimationWater;
      break;
    case ATTR_ELEMENTAL_SKULL_FIRE:
      mSprite->Name("ENEMY FIRE ELEMENTAL SKULL");
      mSprite->mElement = ELEMENT_FIRE;
      mSprite->vx = 1.15;
      mSprite->vy = -.3;
      mAnimation = idleAnimationFire;
      break;
    case ATTR_ELEMENTAL_SKULL_ENERGY:
      mSprite->Name("ENEMY ENERGY ELEMENTAL SKULL");
      mSprite->mElement = ELEMENT_ENERGY;
      mSprite->vx = -.6;
      mSprite->vy = .4;
      mAnimation = idleAnimationEnergy;
      break;
    default:
      Panic("Unsupported elemental skull type");
  }

  mSprite->StartAnimation(mAnimation);

  mSpriteX = mSprite->x;
  mSpriteY = mSprite->y;
}

GElementalSkullProcess::~GElementalSkullProcess()  = default;

void GElementalSkullProcess::Idle(DIRECTION aDirection) {
  if (mSprite->AnimDone()) {
    mSprite->StartAnimation(mAnimation);
  }
}

TBool GElementalSkullProcess::BasicDamageCheck() {
  if (mInvulnerable || !mSprite->TestAndClearCType(STYPE_PBULLET)) {
    return EFalse;
  }

  DoDamage(0);
  StartBlink(BLINK_TIME);
  return ETrue;
}

TBool GElementalSkullProcess::SpellDamageCheck() {
  if (mInvulnerable || !mSprite->TestAndClearCType(STYPE_SPELL)) {
    return EFalse;
  }

  TBool shouldTakeDamage = DAMAGE_MATRIX[mSprite->mElement - 1][GPlayer::mEquipped.mSpellBookElement - 1];
  if (shouldTakeDamage) {
    DoDamage(999, EFalse);
    StartBlink(BLINK_TIME);
    return ETrue;
  }

  return EFalse;
}

void GElementalSkullProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  const TFloat lastVx = mSprite->vx;
  const TFloat lastVy = mSprite->vy;
  GGruntProcess::NewState(aState, DIRECTION_DOWN);
  mSprite->vx = lastVx;
  mSprite->vy = lastVy;
}

void GElementalSkullProcess::Hit(DIRECTION aDirection) {
  StartBlink(BLINK_TIME);
}

void GElementalSkullProcess::Death(DIRECTION aDirection) {
  mSprite->ClearFlags(SFLAG_ANIMATE);

  // taking the current unreleased instance into account
  TInt8 instances = -1;
  for (BProcess *p = mGameState->mProcessList.First(); !mGameState->mProcessList.End(p); p = mGameState->mProcessList.Next(p)) {
    TUint16 attr = ((GProcess*)p)->mAttribute;
    if (attr >= ATTR_ELEMENTAL_SKULL_EARTH && attr <= ATTR_ELEMENTAL_SKULL_ENERGY) {
      instances++;
    }
  }

  if (instances > 0) {
    return;
  }

  TInt objectCount = mGameState->mGamePlayfield->mObjectCount;
  BObjectProgram *program = mGameState->mGamePlayfield->mObjectProgram;

  for (TInt ip = 0; ip < objectCount; ip++) {
    const TUint16 op = program[ip].mCode & TUint32(0xffff);
    if (op >= ATTR_ELEMENTAL_SKULL_EARTH && op <= ATTR_ELEMENTAL_SKULL_ENERGY) {
      mGameState->EndProgram(ip);
    }
  }
}

TBool GElementalSkullProcess::HitState() {
  mSprite->ClearFlags(SFLAG_ANIMATE);
  return GGruntProcess::HitState();
}

TBool GElementalSkullProcess::RunBefore() {
  mSprite->ResetShadow();
  return GGruntProcess::RunBefore();
}

TBool GElementalSkullProcess::RunAfter() {
//  if (--mTimer == 0) {
//    mTimer = OSCILLATE_MAX_X;
//    mSprite->vx *= -1;
//  }

  if (mSprite->vx > 0 && mSprite->x > mSpriteX + OSCILLATE_MAX_X) {
    mSprite->vx *= -1;
  }
  else if (mSprite->vx < 0 && mSprite->x < mSpriteX - OSCILLATE_MAX_X) {
    mSprite->vx *= -1;
  }

  if (mSprite->vy > 0 && mSprite->y > mSpriteY + OSCILLATE_MAX_Y) {
    mSprite->vy *= -1;
  }
  else if (mSprite->vy < 0 && mSprite->y < mSpriteY - OSCILLATE_MAX_Y) {
    mSprite->vy *= -1;
  }

  return GGruntProcess::RunAfter();
}
