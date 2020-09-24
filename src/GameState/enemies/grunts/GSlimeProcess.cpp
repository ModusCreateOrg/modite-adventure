#include "GSlimeProcess.h"
#include "GPlayer.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt16 IDLE_SPEED = 3 * FACTOR;
const TInt16 SELECT_SPEED = 3 * FACTOR;
const TInt16 TAUNT_SPEED = 5 * FACTOR;
const TInt16 ATTACK_SPEED = 2 * FACTOR;
const TInt16 HIT_SPEED = 2 * FACTOR;
const TInt16 WALK_SPEED = 4 * FACTOR;
const TInt16 DEATH_SPEED = 5;

const TFloat VELOCITY = PLAYER_VELOCITY / 4;

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


static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_SLIME_IDLE + 0),
  AEND,
};


/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, -2),
  ASTEP(ATTACK_SPEED * 3, IMG_SLIME_ATTACK_DOWN + 0),
  ADELTA(0, 4),
  ASTEP(ATTACK_SPEED, IMG_SLIME_WALK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 32, 32, 48),
  ADELTA(0, 32),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_DOWN + 1),
  ADELTA(0, 32),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(0, 32),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_DOWN + 3),
  ADELTA(0, -1),
  ASTEP(ATTACK_SPEED, IMG_SLIME_WALK_DOWN + 0),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 0),
  ADELTA(0, -2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 1),
  ADELTA(0, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 2),
  ADELTA(0, -2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 3),
  AEND
};

static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, -2),
  ASTEP(DEATH_SPEED, IMG_SLIME_WALK_DOWN + 1),
  AEND
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(SLIME_SLOT),
  AFLIP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(SLIME_SLOT),
  AFLIP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 3),
  AEND
};


static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(2, 0),
  AFLIP(ATTACK_SPEED * 3, IMG_SLIME_ATTACK_RIGHT + 0),
  ADELTA(-1, 0),
  AFLIP(ATTACK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-32, 0, 52, 24),
  ADELTA(-8, 2),
  AFLIP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 1),
  ADELTA(-7, 2),
  AFLIP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(-20, 2),
  AFLIP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 3),
  ADELTA(-1, 0),
  AFLIP(ATTACK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(-2 ,0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 0),
  ADELTA(9, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 1),
  ADELTA(7, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 2),
  ADELTA(-2, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 3),
  AEND
};

static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(2, 0),
  AFLIP(DEATH_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  AEND
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */

static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_SLIME_ATTACK_RIGHT + 0),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 0, 52, 24),
  ADELTA(2, 2),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(2, 2),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(-4, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 0),
  ADELTA(-2, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 1),
  ADELTA(-4, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 2),
  ADELTA(-2, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_RIGHT + 3),
  AEND
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(-2, 0),
  ASTEP(DEATH_SPEED, IMG_SLIME_WALK_RIGHT + 0),
  AEND
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */

static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SLIME_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(SLIME_SLOT),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_SLIME_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED * 3, IMG_SLIME_ATTACK_UP + 0),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_WALK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, -10, 32, 50),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_UP + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SLIME_ATTACK_UP + 3),
  ADELTA(0, 1),
  ASTEP(ATTACK_SPEED, IMG_WALK_UP + 0),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_UP + 0),
  ADELTA(0, 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_UP + 1),
  ADELTA(0, 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_UP + 2),
  ADELTA(0, 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_UP + 3),
  AEND
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(SLIME_SLOT),
  ADELTA(0, 2),
  ASTEP(DEATH_SPEED, IMG_SLIME_WALK_UP + 1),
  AEND
};



static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(SLIME_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 3),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_SLIME_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GSlimeProcess::GSlimeProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aIp, aX, aY, SLIME_SLOT, aParams, VELOCITY, ATTR_SLIME) {
  mStateTimer = 0;
  mSprite->Name("ENEMY SLIME");
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_SLIME_BMP_SPRITES);
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->cy = 0;
  mSprite->ResetShadow();
  SetStatMultipliers(2.0, 2.5, 1.0);
  mRangeX = mRangeY = 32;
}

GSlimeProcess::~GSlimeProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GSlimeProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GSlimeProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}


void GSlimeProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GSlimeProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GSlimeProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GSlimeProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GSlimeProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}
