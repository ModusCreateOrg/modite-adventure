#include "GSpiderProcess.h"
#include "GStatProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 2 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt TAUNT_SPEED = 3 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 2 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = PLAYER_VELOCITY * 1.25;

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

ANIMSCRIPT idleAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ADELTA(0, -2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 1),
  ALOOP,
};


static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 0),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 1),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 2),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 4),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 5),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 6),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 5),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 6),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 5),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 4),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 2),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 1),
  ADELTA(0, -2),
  ASTEP(TAUNT_SPEED, IMG_SPIDER_IDLE + 0),
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ADELTA(0, -2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 1),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 0),
  ADELTA(0, -2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 2),
  ADELTA(0, -2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -2),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 0),
  ADELTA(0, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 8, 32, 32),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(0, 4),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -4),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 0),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 1),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 2),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -4),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 0),
  AEND,
};

static ANIMSCRIPT hitSpellDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  AEND,
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ADELTA(-2, 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASIZE(0, 0, 32, 24),
  ADELTA(-6, 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ADELTA(-2, 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASIZE(0, 0, 32, 24),
  ADELTA(-2, 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(-3, -2),
  AFLIP(ATTACK_SPEED * 2, IMG_SPIDER_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 0, 40, 32),
  ADELTA(-12, -2),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 1),
  ASIZE(0, 0, 32, 24),
  ATYPE(STYPE_ENEMY),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 2),
  ADELTA(-6, -4),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 3),
  ADELTA(-6, 0),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(-2, -2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  ADELTA(-2, -2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ADELTA(0, -2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ADELTA(0, -2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AEND,
};
static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(-2, -2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT hitSpellLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AEND,
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(-2, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -2),
  ASTEP(ATTACK_SPEED * 2, IMG_SPIDER_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-7, 0, 44, 32),
  ADELTA(0, -2),
  ASTEP(ATTACK_SPEED * 2, IMG_SPIDER_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 24),
  ADELTA(0, -2),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 2),
  ADELTA(0, -4),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 3),
  ADELTA(-1, 0),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  ADELTA(-1, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ADELTA(-1, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ADELTA(-2, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitSpellRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AEND,
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ADELTA(0, -3),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 3),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -1),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 0),
  ADELTA(0, -3),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -1),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 2),
  ADELTA(0, -3),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -1),
  ASTEP(ATTACK_SPEED * 2, IMG_SPIDER_ATTACK_UP + 0),
  ADELTA(0, -1),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 32, 44),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 32),
  ADELTA(0, -1),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 2),
  ADELTA(0, -3),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 3),
  ADELTA(0, -1),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 0),
  ADELTA(0, -3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 1),
  ADELTA(0, -3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 2),
  ADELTA(0, -3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ADELTA(0, -3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 0),
  AEND,
};

static ANIMSCRIPT hitSpellUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};
static ANIMSCRIPT* hitSpellAnimations[] = {hitSpellUpAnimation, hitSpellDownAnimation, hitSpellLeftAnimation, hitSpellRightAnimation};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GSpiderProcess::GSpiderProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
  : GGruntProcess(aGameState, aIp, aX, aY, SPIDER_SLOT, aParams, VELOCITY, ATTR_SPIDER) {
  mStateTimer = 0;
  mSprite->Name("ENEMY SPIDER");
  mSprite->h = 24;
  mSprite->cy = 0;
  mSprite->ResetShadow();
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_SPIDER_BMP_SPRITES);
  SetStatMultipliers(0.9, 0.9, 0.9);

//  printf("%s -> %i\n", mSprite->Name(), mSprite->mHitPoints);
}

GSpiderProcess::~GSpiderProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GSpiderProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);

}

void GSpiderProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GSpiderProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(2, 4)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GSpiderProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GSpiderProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GSpiderProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitSpellAnimations, aDirection);
}

void GSpiderProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}

