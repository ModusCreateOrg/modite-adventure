/**
  * Final Boss Process
  */

#include "GFinalBossProcess.h"
#include "GFinalBossProjectileProcess.h"
#include "GFinalBossPillarProcess.h"
#include "GFinalBossDeathProcess.h"
#include "common/GSpellOverlayProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

enum {
  STATE_INITIALIZE,
  STATE_IDLE,
  STATE_WALK,
  STATE_CHARGE,
  STATE_LEAP,
  STATE_PROJECTILE,
  STATE_SPELL,
  STATE_STUN,
  STATE_NEXT_PHASE,
  STATE_DEATH,
};

#ifdef DEBUGME
static const char *direction_names[] = {
  "DIRECTION_UP",
  "DIRECTION_DOWN",
  "DIRECTION_LEFT",
  "DIRECTION_RIGHT",
  "DIRECTION_SPELL",
};
#endif

const TFloat WALK_VELOCITY = 1.4;
const TFloat CHARGE_VELOCITY = 2 * PLAYER_VELOCITY;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;
const TInt INITIALIZE_HEALTH_RATE = 20 / FACTOR;
const TInt HOP_DURATION = FRAMES_PER_SECOND / 4;
const TFloat HOP_FRICTION = 0.2 / TFloat(FACTOR);

const TInt16 WALK_SPEED = 10;
const TInt16 PROJECTILE_SPEED = 30;
const TInt16 HIT_SPEED = 5;
const TInt16 IDLE_SPEED = 1;
const TInt16 INITIALIZE_SPEED = 20;
const TInt16 ATTACK_SPEED = 5;
const TInt BLINK_DURATION = FRAMES_PER_SECOND / 4;

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_FINAL_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(BOSS_SLOT),

  AFILL(COLOR_TEXT),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),

  AFILL(COLOR_TEXT),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),

  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 2),
  AEND,
};

static ANIMSCRIPT initializeAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(INITIALIZE_SPEED * 4, IMG_FINAL_BOSS_SPELL_START),
  ADELTA(0, -2),
  ASTEP(INITIALIZE_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ASTEP(INITIALIZE_SPEED, IMG_FINAL_BOSS_SPELL_START + 1),
  ADELTA(0, 0),
  ASTEP(INITIALIZE_SPEED, IMG_FINAL_BOSS_SPELL_START + 1),
  ASTEP(INITIALIZE_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ALOOP,
};

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_FINAL_BOSS_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  AEND,
};
static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  AEND,
};
static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};
static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};
static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT landUpAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_FINAL_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT landDownAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_FINAL_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT landLeftAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 2),
  AFLIP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED * 5, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT landRightAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT projectileAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT teleportAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT teleportAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT teleportAnimation3[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT earthProjectileAnimation[] = {
  ABITMAP(BOSS_SLOT),
  // Common start frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  //Elemental frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 11),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 12),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 11),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 12),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 13),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 14),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 15),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE),
  AEND,
};

static ANIMSCRIPT energyProjectileAnimation[] = {
  ABITMAP(BOSS_SLOT),
  // Common start frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  //Elemental frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 16),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 17),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 16),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 17),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 18),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 19),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 20),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE),
  AEND,
};

static ANIMSCRIPT fireProjectileAnimation[] = {
  ABITMAP(BOSS_SLOT),
  // Common start frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  //Elemental frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 21),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 22),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 21),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 22),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 23),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 24),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 25),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE),
  AEND,
};

static ANIMSCRIPT waterProjectileAnimation[] = {
  ABITMAP(BOSS_SLOT),
  // Common start frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 9),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 10),
  //Elemental frames
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 26),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 27),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 26),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 27),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 28),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 29),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE + 30),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_IDLE),
  AEND,
};

static ANIMSCRIPT fireDownAnimation[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT fireLeftAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT fireRightAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* landAnimations[] = {landUpAnimation, landDownAnimation, landLeftAnimation, landRightAnimation};

// constructor
GFinalBossProcess::GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams)
    : GBossProcess(aGameState, aX, aY, BOSS_SLOT, aParams) {
  mSprite->Name("Final Boss");
  mSprite->StartAnimation(initializeAnimation);
  mSprite->h = 32;
  mSprite->cy = 8;
  mSprite->ResetShadow();
  mHitTimer = HIT_SPAM_TIME;
  mAttackType = EFalse;
  SetAttackTimer();
  SetStatMultipliers(10.0, 2.0, 0.0);
  mState = STATE_INITIALIZE;
  mShieldProcess = ENull;
}

// destructor
GFinalBossProcess::~GFinalBossProcess() {
#ifdef DEBUGME
  printf("GFinalBoss died!\n ");
#endif
}

void GFinalBossProcess::RaiseShield() {
  if (mShieldProcess) {
    LowerShield();
  }
  mShieldProcess = new GFinalBossShieldProcess(mGameState, EARTH_FINAL_BOSS_PILLAR_SLOT + Random(0, 3));
  mGameState->AddProcess(mShieldProcess);
  mShieldProcess->UpdateCenter(mSprite->Center());
  mInvulnerable = ETrue;
}

void GFinalBossProcess::LowerShield() {
  if (mShieldProcess) {
    mShieldProcess->KillShield();
    mShieldProcess = ENull;
  }
  mInvulnerable = EFalse;
}

void GFinalBossProcess::SetAttackTimer() {
  mAttackTimer = 5 * 60;
}

void GFinalBossProcess::Idle(DIRECTION aDirection) {
  mSprite->StartAnimation(idleDownAnimation);
}

void GFinalBossProcess::Walk(DIRECTION aDirection) {
  mSprite->MoveInDirection(WALK_VELOCITY, aDirection);
  mSprite->StartAnimationInDirection(mStep ? walkAnimations2 : walkAnimations1, aDirection);
  mStep = !mStep;
}

void GFinalBossProcess::Charge(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(mStep ? walkAnimations2 : walkAnimations1, aDirection);
  mStep = !mStep;
}

void GFinalBossProcess::Land(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(landAnimations, aDirection);
}

void GFinalBossProcess::Projectile(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mDirection = aDirection;
  switch (aDirection) {
    case 0:
#ifdef DEBUGME
      printf("earthProjectileAnimation\n");
#endif
      mSprite->StartAnimation(earthProjectileAnimation);
      break;
    case 1:
#ifdef DEBUGME
      printf("waterProjectileAnimation\n");
#endif

      mSprite->StartAnimation(waterProjectileAnimation);
      break;
    case 2:
#ifdef DEBUGME
      printf("fireProjectileAnimation\n");
#endif
      mSprite->StartAnimation(fireProjectileAnimation);
      break;
    case 3:
#ifdef DEBUGME
      printf("energyProjectileAnimation\n");
#endif
      mSprite->StartAnimation(energyProjectileAnimation);
      break;
    default:
#ifdef DEBUGME
      printf("DEFAULT energyProjectileAnimation\n");
#endif

      mSprite->StartAnimation(earthProjectileAnimation);
      break;
  }
}

void GFinalBossProcess::Spell(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
  mSpellCounter++;
  auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1, 0, 0, 0);
  mGameState->AddProcess(p);
}

void GFinalBossProcess::Death(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->type = STYPE_OBJECT;
  mSprite->StartAnimation(deathAnimation);
  // get coordinates for explosion placement
  TRect r;
  mSprite->GetRect(r);
  //  r.Dump();
  mDeathCounter = 10;
  for (TInt delay = 0; delay < mDeathCounter; delay++) {
#ifdef DEBUGME
    printf("DEATH SPRITE @ %d,%d\n", r.x1, r.x2);
#endif
    auto *p = new GFinalBossDeathProcess(mGameState, this, r.x1, r.y1 - 32, delay);
    mGameState->AddProcess(p);
  }
}

void GFinalBossProcess::SetState(TInt aNewState, DIRECTION aNewDirection) {
  mStep = 0;
  mState = aNewState;
  mDirection = aNewDirection;
  switch (aNewState) {
    case STATE_IDLE:
      mSprite->vx = mSprite->vy = 0;
      mStateTimer = 2 * 60;
      SetAttackTimer();
      Idle(mDirection);
      break;
    case STATE_WALK:
      Walk(mDirection);
      break;
    case STATE_CHARGE:
      mSprite->vx = mSprite->vy = 0;
      mStateTimer = -FRAMES_PER_SECOND;
      Charge(mDirection);
      break;
    case STATE_PROJECTILE:
      Projectile(mDirection);
      break;
    case STATE_SPELL:
      Spell(mDirection);
      break;
    case STATE_DEATH:
      Death(mDirection);
      break;
    default:
      Panic("invalid state");
      break;
  }
}

TBool GFinalBossProcess::MaybeHit() {
  if (SpellDamageCheck()) {
#ifdef DEBUGME
    if (mHitPoints <= 0) {
      printf("FINAL BOSS DEATH\n");
    }
#endif
    StartBlink(BLINK_DURATION);
    SetState(STATE_SPELL, mSprite->mDirection);
    return ETrue;
  }

  if (BasicDamageCheck()) {
    StartBlink(BLINK_DURATION);
  }

  if (mHitPoints <= 0) {
    if (mCurrentHealthBar > 1) {
      mCurrentHealthBar--;
      mHitPoints = mMaxHitPoints;
      SetState(STATE_NEXT_PHASE, mSprite->mDirection);
    } else {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    return ETrue;
  }

  mSprite->ClearCType(STYPE_PLAYER);

  return EFalse;
}

TBool GFinalBossProcess::MaybeAttack() {
  if (!mGameState->IsGameOver() && --mAttackTimer < 0) {
#ifdef DEBUGME
    printf("Attack! %s\n", mAttackType ? "TELEPORT" : "PROJECTILE");
#endif
    //    SetState(STATE_TELEPORT, mDirection);
    SetState(mAttackType ? STATE_CHARGE : STATE_PROJECTILE, mDirection);
    mAttackType = !mAttackType;
    return ETrue;
  }
  return EFalse;
}

TBool GFinalBossProcess::MaybeBounce() {
  TFloat vx = mSprite->vx, vy = mSprite->vy;
  TBool bouncedX = EFalse, bouncedY = EFalse;

  if (vx > 0 ? !mSprite->CanWalkInDirection(DIRECTION_RIGHT, vx, vy) :
      !mSprite->CanWalkInDirection(DIRECTION_LEFT, vx, vy)) {
    mSprite->vx = -vx;
    bouncedX = ETrue;
    mSprite->x = mSprite->mLastX;
  }

  if (vy > 0 ? !mSprite->CanWalkInDirection(DIRECTION_DOWN, vx, vy) :
      !mSprite->CanWalkInDirection(DIRECTION_UP, vx, vy)) {
    mSprite->vy = -vy;
    bouncedY = ETrue;
    mSprite->y = mSprite->mLastY;
  }

  if (bouncedX || bouncedY) {
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_BOUNCE_WALL_WAV, 4);
  }

  return bouncedX || bouncedY;
}

TBool GFinalBossProcess::InitializeState() {
  if (!mInvulnerable) {
    RaiseShield();
    return ETrue;
  }
  mHitPoints += INITIALIZE_HEALTH_RATE;
  if (mHitPoints > mMaxHitPoints) {
    if (mHealthBarCount < 4) {
      mHitPoints = 0;
      mHealthBarCount++;
      mCurrentHealthBar++;
    } else {
      mHitPoints = mMaxHitPoints;
      SetState(STATE_IDLE, DIRECTION_DOWN);
    }
  }

  return ETrue;
}

TBool GFinalBossProcess::IdleState() {
  if (MaybeAttack()) {
    return ETrue;
  }
  if (--mStateTimer < 1) {
    mStep = 0;
    SetState(STATE_WALK, DIRECTION_RIGHT);
  }
  return ETrue;
}

TBool GFinalBossProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (MaybeAttack()) {
    return ETrue;
  }

  if (!mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
#ifdef DEBUGME
    printf("Final Boss can't walk direction %s\n", direction_names[mDirection]);
#endif
    SetState(STATE_WALK, GAnchorSprite::RotateDirection(mDirection, 2));
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    mStep = 1 - mStep;
    Walk(mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::ChargeState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mStateTimer < 0) {
    mStateTimer++;
    TFloat xx = GPlayer::mSprite->Center().x - mSprite->Center().x,
      yy = GPlayer::mSprite->Center().y - mSprite->Center().y;
    DIRECTION direction = GAnchorSprite::VectorToDirection(xx, yy);
    if (mSprite->AnimDone() || mSprite->mDirection != direction) {
      mSprite->mDirection = direction;
      Charge(direction);
    }
    if (mStateTimer == 0) {
      mSprite->vx = CHARGE_VELOCITY * xx / hypot(xx, yy);
      mSprite->vy = CHARGE_VELOCITY * yy / hypot(xx, yy);
      mSprite->type = STYPE_EBULLET;
    }
  } else if (mStateTimer == 0) {
    if (mSprite->AnimDone()) {
      Charge(mSprite->mDirection);
    }
    if (MaybeBounce()) {
      mStateTimer++;
      mSprite->vx *= WALK_VELOCITY / CHARGE_VELOCITY;
      mSprite->vy *= WALK_VELOCITY / CHARGE_VELOCITY;
      LowerShield();
    }
    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      SetState(STATE_IDLE, mSprite->mDirection);
    }
  } else if (mStateTimer < HOP_DURATION) {
    mStateTimer++;
    mSprite->mDy = GRAVITY * .5 * mStateTimer * (mStateTimer - HOP_DURATION);
    MaybeBounce();
  } else if (mStateTimer == HOP_DURATION) {
    Land(mSprite->mDirection);
    mStateTimer++;
    MaybeBounce();
  } else {
    mSprite->vx *= 1 - HOP_FRICTION;
    mSprite->vy *= 1 - HOP_FRICTION;
    if (mSprite->AnimDone()) {
      SetState(STATE_IDLE, mSprite->mDirection);
    }
    MaybeBounce();
  }

  mSprite->ClearCType(STYPE_PLAYER);

  return ETrue;
}

TBool GFinalBossProcess::ProjectileState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (mSprite->AnimDone()) {
    // fire!
    TInt16 type = 0;
    switch (mDirection) {
      case 0:
        type = ATTR_FINAL_BOSS_EARTH;
        break;
      case 1:
        type = ATTR_FINAL_BOSS_WATER;
        break;
      case 2:
        type = ATTR_FINAL_BOSS_FIRE;
        break;
      case 3:
        type = ATTR_FINAL_BOSS_ENERGY;
        break;

      default:
        type = ATTR_FINAL_BOSS_EARTH;
        break;
    }
    TFloat xx = mSprite->x + 48,
           yy = mSprite->y;
    // fire 10 projectiles in a circle pattern with the boss in the center
    for (TFloat angle = 0; angle < 360; angle += 360 / 10) {
      mGameState->AddProcess(new GFinalBossProjectileProcess(mGameState, xx, yy, angle, type));
    }
    SetState(STATE_IDLE, mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::SpellState() {
  if (mSprite->AnimDone() && mSpellCounter <= 0) {
    if (mHitPoints <= 0) {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    else {
      mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      SetState(STATE_IDLE, mSprite->mDirection);
    }
  }
  return ETrue;
}

TBool GFinalBossProcess::DeathState() {
  if (mDeathCounter <= 3) {
    gGame->SetState(GAME_STATE_VICTORY);
    return EFalse;
  }
  // maybe drop item
  return ETrue;
}

TBool GFinalBossProcess::RunBefore() {
  switch (mState) {
    case STATE_INITIALIZE:
      return InitializeState();
    case STATE_IDLE:
      return IdleState();
    case STATE_WALK:
      return WalkState();
    case STATE_CHARGE:
      return ChargeState();
    case STATE_PROJECTILE:
      return ProjectileState();
    case STATE_SPELL:
      return SpellState();
    case STATE_DEATH:
      return DeathState();
    default:
      Panic("GFinalBossProcess: Invalid state (%d)\n", mState);
  }
  return ETrue;
}

TBool GFinalBossProcess::RunAfter() {
  UpdateBlink();
  if (mShieldProcess) {
    mShieldProcess->UpdateCenter(mSprite->Center());
  }
  return ETrue;
}
