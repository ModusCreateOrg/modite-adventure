#include "GTrollProcess.h"
#include "GPlayer.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 3 * FACTOR;
const TInt TAUNT_SPEED = 3 * FACTOR;
const TInt SELECT_SPEED = 3 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 1;

const TFloat VELOCITY = PLAYER_VELOCITY * 0.55;

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

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_TROLL_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_TROLL_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_TROLL_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 0),
  AEND,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_WALK_DOWN + 0),
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
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, -1),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 0),
  ADELTA(0, -1),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, -1),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-14, 16, 48, 50),
  ADELTA(0, 17),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(3, -1),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 2),
  ADELTA(3, -1),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, -1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 0),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT deathDownAnimation[] = {
    ABITMAP(TROLL_SLOT),
    ADELTA(0, -1),
    ASTEP(DEATH_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
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
  ABITMAP(TROLL_SLOT),
  ADELTA(-9, -1),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-9, -1),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-7, -1),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(11, -1),
  AFLIP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-32, 0, 52, 32),
  ADELTA(-19, 9),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(-6, -1),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(-8, -1),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-3, -1),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 0),
  ADELTA(-7, -1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
  ADELTA(-7, -1),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 2),
  ADELTA(-3, -1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-7, -1),
  AFLIP(DEATH_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
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
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-9, 0, 52, 32),
  ADELTA(6, 10),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
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
  ABITMAP(TROLL_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 3),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-20, 0, 48, 50),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 3),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(TROLL_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 1),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_TROLL_DAMAGE_UP + 2),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(0, -1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 0),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  ADELTA(0, -1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 0),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  AFILL(-1),
  ADELTA(0, -1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GTrollProcess::GTrollProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aIp, aX, aY, TROLL_SLOT, aParams, VELOCITY, ATTR_TROLL) {
  mSprite->Name("ENEMY TROLL");
  SetStatMultipliers(5.0, 5.0, 5.0);
  mRangeX = mRangeY = 16;
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_TROLL_BMP_SPRITES);
  mSprite->cy = 0;
  mSprite->h = 16;
}

GTrollProcess::~GTrollProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GTrollProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GTrollProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GTrollProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GTrollProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GTrollProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GTrollProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GTrollProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}
