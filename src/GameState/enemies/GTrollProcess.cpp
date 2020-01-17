#include "GTrollProcess.h"
#include "GPlayer.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt TAUNT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = PLAYER_VELOCITY * 0.3;

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
  ABITMAP(TROLL_SLOT),
  ALABEL,
  ASTEP(40, IMG_TROLL_IDLE),
  ASTEP(4, IMG_TROLL_IDLE + 1),
  ASTEP(40, IMG_TROLL_IDLE + 2),
  ASTEP(4, IMG_TROLL_IDLE + 1),
  ALOOP,
};

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
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 7),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_TROLL_IDLE + 7),
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
  ALABEL,
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 0),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(8, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 3),
  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 20, 32, 32),
  ADELTA(2, 16),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 16),
  ADELTA(10, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 2),
  ADELTA(10, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 3),
  ADELTA(10, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_DOWN + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_WALK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(TROLL_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  AFILL(-1),
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
  ALABEL,
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-8, 0),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  ADELTA(6, 0),
  AFLIP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-24, 4, 48, 16),
  ADELTA(-14, 10),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 16),
  ADELTA(-8, 0),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(-8, 0),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  ADELTA(-8, 0),
  AFLIP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(-8, 0),
  AFLIP(ATTACK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-8, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AFILL(-1),
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
  ALABEL,
  ADELTA(4, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(4, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(4, 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-8, 4, 48, 16),
  ADELTA(12, 10),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 16),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 3),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_RIGHT + 2),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(2, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_RIGHT + 3),
  AFILL(-1),
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
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 0),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_TROLL_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 3, IMG_TROLL_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 16),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 2),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 3),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_TROLL_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(TROLL_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_UP + 3),
  AFILL(-1),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(TROLL_SLOT),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_TROLL_DAMAGE_DOWN + 3),
  AEND,
};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GTrollProcess::GTrollProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GEnemyProcess(aGameState, aIp, TROLL_SLOT, aParams, VELOCITY, ATTR_TROLL) {
  mSprite->Name("ENEMY TROLL");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->SetStatMultipliers(5.0, 5.0, 5.0);
  mRangeX = mRangeY = 16;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_TROLL_BMP_SPRITES);
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GTrollProcess::~GTrollProcess() {
  if (mSprite) {
    mGameState->RemoveSprite(mSprite);
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GTrollProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(idleUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(idleDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(idleLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(idleRightAnimation);
      break;
    default:
      Panic("GTrollProcess no idle direction\n");
      break;
  }
}

void GTrollProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      mSprite->vy = -VELOCITY;
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
    default:
      Panic("GTrollProcess no walk direction\n");
      break;
  }
}

void GTrollProcess::Attack(DIRECTION aDirection) {
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(attackUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(attackDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(attackLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(attackRightAnimation);
      break;
    default:
      Panic("GTrollProcess no attack direction\n");
      break;
  }
}

void GTrollProcess::Hit(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(hitUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(hitDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(hitLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(hitRightAnimation);
      break;
    case DIRECTION_SPELL:
      mSprite->StartAnimation(hitSpellAnimation);
      break;
    default:
      Panic("GTrollProcess no Hit direction\n");
      break;
  }
}

void GTrollProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}
