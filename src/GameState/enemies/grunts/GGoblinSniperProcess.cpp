#include "GGoblinSniperProcess.h"
#include "GEnemyProjectileProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 10 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt TAUNT_SPEED = 3 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 2 * FACTOR;
const TInt DEATH_SPEED = 1;

const TFloat VELOCITY = 1.45 * PLAYER_VELOCITY;

const TFloat ATTACK_DISTANCE = 6 * TILESIZE;
const TFloat FLEE_DISTANCE = 3 * TILESIZE;

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
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ASTEP(TAUNT_SPEED * 2, IMG_GOBLIN_SNIPER_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED - FACTOR, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 7),
  ASTEP(TAUNT_SPEED - FACTOR, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED - FACTOR, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 7),
  ASTEP(TAUNT_SPEED - FACTOR, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 4),
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
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 4),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 0),
  AEND
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 4),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 0),
  ADELTA(0, 4),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 4),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 2),
  ADELTA(0, 4),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 0),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 1),
  ADELTA(0, 4),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 2),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 3),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ADELTA(-1, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  AEND
};


static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
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
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-11, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-11, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ADELTA(-13, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-11, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 2),
  ADELTA(-11, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-7, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 0),
  ADELTA(-7, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 1),
  ADELTA(-7, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 2),
  ADELTA(-5, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-15, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
  ADELTA(-4, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 1),
  ADELTA(-4, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 2),
  ADELTA(-8, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AEND
};

static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-11, 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
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
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 2),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 0),
  ADELTA(-1, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 1),
  ADELTA(-1, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 2),
  ADELTA(-1, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AEND
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
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
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 0),
  AEND
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 0),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_UP + 0),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_UP + 3),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 3),
  AEND
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 0),
  AEND
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(GOBLIN_SNIPER_SLOT),
  ADELTA(0, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ADELTA(-1, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ADELTA(0, -2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  ADELTA(0, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ADELTA(-1, -2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ADELTA(0, -2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GGoblinSniperProcess::GGoblinSniperProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam)
    : GGruntProcess(aGameState, aIp, aX, aY, GOBLIN_SNIPER_SLOT, aParam, VELOCITY, ATTR_GOBLIN_SNIPER) {
  mGameState = aGameState;

  mSprite->Name("ENEMY GOBLIN SNIPER");
  mSprite->cy = 4;
  mSprite->cx = -1;
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->ResetShadow();
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_GOBLIN_SNIPER_BMP_SPRITES);
}

GGoblinSniperProcess::~GGoblinSniperProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GGoblinSniperProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);

}

void GGoblinSniperProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GGoblinSniperProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;

  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GGoblinSniperProcess::Attack(DIRECTION aDirection) {

  // fire 1-3 projectiles
  TFloat xx = mSprite->x,
         yy = mSprite->y;

  // Angles are in radians
  const TFloat angleToPlayer = atan2(GPlayer::mSprite->y - yy, GPlayer::mSprite->x - xx);

  TInt attackType = Random() & TUint8(3);

  // 25% chance for multi-arrow attack
  if (attackType < 3) {
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angleToPlayer, PROJECTILE_ARROW_SLOT));
  } else {
    const TFloat step = 22.5 * (M_PI/180);
    const TFloat angles[3] = { angleToPlayer, angleToPlayer + step, angleToPlayer - step };
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[0], PROJECTILE_ARROW_SLOT));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[1], PROJECTILE_ARROW_SLOT));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[2], PROJECTILE_ARROW_SLOT));
  }

  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GGoblinSniperProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GGoblinSniperProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GGoblinSniperProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}

TBool GGoblinSniperProcess::MaybeAttack() {
  if (mGameState->IsGameOver() || GPlayer::mInvulnerable) {
    return EFalse;
  }

  const TFloat distance = sqrt(pow(GPlayer::mSprite->x - mSprite->x, 2) + pow(GPlayer::mSprite->y - mSprite->y, 2));

#ifdef DEBUG_MODE
  // printf("Distance from sniper to player %f\n", distance);
#endif

  if (distance > ATTACK_DISTANCE || distance < FLEE_DISTANCE) {
    return EFalse;
  }

  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  GPlayer::mSprite->GetRect(hisRect);

  if (ABS(myRect.y2 - hisRect.y2) < ABS(myRect.x1 - hisRect.x1)) {
    if (myRect.x1 >= hisRect.x2) {
      // to right of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_LEFT);
      }
      return ETrue;
    }
    if (myRect.x2 <= hisRect.x1) {
      // to left of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_RIGHT);
      }
      return ETrue;
    }
  } else {
    if (myRect.y1 >= hisRect.y2) {
      // below player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_UP);
      }
      return ETrue;
    }
    if (myRect.y2 <= hisRect.y1) {
      // above player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_DOWN);
      }
      return ETrue;
    }
  }

  mAttackTimer = 1;
  return EFalse;
}

TBool GGoblinSniperProcess::RunAfter() {
  // Make sniper stationary (turret) for debugging
  // mSprite->vx = 0;
  // mSprite->vy = 0;
  return GGruntProcess::RunAfter();
}
