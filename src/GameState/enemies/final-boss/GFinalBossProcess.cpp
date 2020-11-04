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
  STATE_PILLAR,
  STATE_STUN,
  STATE_RESET_SHIELD,
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
const TFloat CHARGE_VELOCITY = 5 * PLAYER_VELOCITY;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;
const TInt INITIALIZE_HEALTH_RATE = 20 / FACTOR;
const TInt HOP_DURATION = FRAMES_PER_SECOND / 4;
const TFloat HOP_FRICTION = 0.2 / TFloat(FACTOR);
const TInt16 LEAP_DURATION = FRAMES_PER_SECOND;
const TInt16 STUN_DURATION = 3.0 * FRAMES_PER_SECOND;
const TInt16 NORMAL_ATTACK_INTERVAL = 5.0 * FRAMES_PER_SECOND;
const TInt16 LAST_PHASE_ATTACK_INTERVAL = 2.0 * FRAMES_PER_SECOND;

const TInt16 WALK_SPEED = 10;
const TInt16 PROJECTILE_SPEED = 30;
const TInt16 HIT_SPEED = 5;
const TInt16 IDLE_SPEED = 1;
const TInt16 INITIALIZE_SPEED = 20;
const TInt16 ATTACK_SPEED = 5;
const TInt BLINK_DURATION = FRAMES_PER_SECOND / 4;

enum {
  CHARGE_ATTACK,
  LEAP_ATTACK,
  PROJECTILE_ATTACK,
  PILLAR_ATTACK,
};

static TInt8 phaseOneAttacks[] = {
  PROJECTILE_ATTACK,
  PROJECTILE_ATTACK,
  CHARGE_ATTACK,
  -1, // sentinel value
};

static TInt8 phaseTwoAttacks[] = {
  LEAP_ATTACK,
  PROJECTILE_ATTACK,
  LEAP_ATTACK,
  CHARGE_ATTACK,
  PROJECTILE_ATTACK,
  -1,
};

static TInt8 phaseThreeAttacks[] = {
  PILLAR_ATTACK,
  PROJECTILE_ATTACK,
  CHARGE_ATTACK,
  PILLAR_ATTACK,
  LEAP_ATTACK,
  PROJECTILE_ATTACK,
  CHARGE_ATTACK,
  -1,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_FINAL_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT stunAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  AFLIP(HIT_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_UP + 1),
  AFLIP(HIT_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 1),
  ASTEP1(IMG_FINAL_BOSS_SPELL_START),
  AEND,
};

static ANIMSCRIPT recoverAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ADELTA(0, 0),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ADELTA(0, -2),
  ASTEP(HIT_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ADELTA(0, 0),
  ASTEP(HIT_SPEED * 4, IMG_FINAL_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT spellFireAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_START),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_START + 1),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_FIRE),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_FINAL_BOSS_SPELL_FIRE + 4),
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
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW);
  mHitTimer = HIT_SPAM_TIME;
  mAttackIndex = 0;
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
  mShieldProcess = new GFinalBossShieldProcess(mGameState, ELEMENT_FIRE);
  mGameState->AddProcess(mShieldProcess);
  mShieldProcess->UpdateCenter(mSprite->Center());
}

void GFinalBossProcess::LowerShield() {
  if (mShieldProcess) {
    mShieldProcess->KillShield();
    mShieldProcess = ENull;
  }
}

void GFinalBossProcess::SetAttackTimer() {
  if (mCurrentHealthBar == 1) {
    mAttackTimer = LAST_PHASE_ATTACK_INTERVAL;
  } else {
    mAttackTimer = NORMAL_ATTACK_INTERVAL;
  }
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

void GFinalBossProcess::Leap(DIRECTION aDirection) {
  gSoundPlayer.TriggerSfx(SFX_MIDBOSS_CHARGE_STEP_WAV, 4);
  mSprite->StartAnimationInDirection(landAnimations, aDirection);
}

void GFinalBossProcess::Land(DIRECTION aDirection) {
  gSoundPlayer.TriggerSfx(SFX_MIDBOSS_LAND_WAV, 4);
  mSprite->StartAnimationInDirection(landAnimations, aDirection);
}

void GFinalBossProcess::Projectile() {
  mSprite->vx = mSprite->vy = 0;
  switch (Random(0, 3)) {
    case 0:
#ifdef DEBUGME
      printf("earthProjectileAnimation\n");
#endif
      mSprite->StartAnimation(earthProjectileAnimation);
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_EARTH_WAV, 4);
      mSprite->mElement = ELEMENT_EARTH;
      break;
    case 1:
#ifdef DEBUGME
      printf("waterProjectileAnimation\n");
#endif

      mSprite->StartAnimation(waterProjectileAnimation);
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_WATER_WAV, 4);
      mSprite->mElement = ELEMENT_WATER;
      break;
    case 2:
#ifdef DEBUGME
      printf("fireProjectileAnimation\n");
#endif
      mSprite->StartAnimation(fireProjectileAnimation);
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_FIRE_WAV, 4);
      mSprite->mElement = ELEMENT_FIRE;
      break;
    case 3:
#ifdef DEBUGME
      printf("energyProjectileAnimation\n");
#endif
      mSprite->StartAnimation(energyProjectileAnimation);
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_ENERGY_WAV, 4);
      mSprite->mElement = ELEMENT_ENERGY;
      break;
    default:
#ifdef DEBUGME
      printf("DEFAULT energyProjectileAnimation\n");
#endif

      mSprite->StartAnimation(earthProjectileAnimation);
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_EARTH_WAV, 4);
      mSprite->mElement = ELEMENT_EARTH;
      break;
  }
}

void GFinalBossProcess::Spell(DIRECTION aDirection) {
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
    case STATE_LEAP:
      mSprite->vx = mSprite->vy = 0;
      mStateTimer = -1;
      Leap(mDirection);
      break;
    case STATE_PROJECTILE:
    case STATE_PILLAR:
      Projectile();
      mStateTimer = 0;
      break;
    case STATE_STUN:
      mSprite->vx = mSprite->vy = 0;
      mStateTimer = -STUN_DURATION;
      mSprite->StartAnimation(stunAnimation);
      break;
    case STATE_RESET_SHIELD:
      mSprite->StartAnimation(spellFireAnimation);
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
    Spell(mDirection);
    return ETrue;
  }

  if (!mShieldProcess && BasicDamageCheck()) {
    if (mState != STATE_STUN && mState != STATE_RESET_SHIELD) {
      SetState(STATE_STUN, mSprite->mDirection);
      return ETrue;
    }
  }

  if (mHitPoints <= 0) {
    if (mCurrentHealthBar > 1) {
      mCurrentHealthBar--;
      mHitPoints = mMaxHitPoints;
      mAttackIndex = 0;
      SetState(STATE_IDLE, mSprite->mDirection);
    } else {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    return ETrue;
  }

  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET | STYPE_SPELL);

  return EFalse;
}

TBool GFinalBossProcess::MaybeAttack() {
  if (!mGameState->IsGameOver() && --mAttackTimer < 0) {
#ifdef DEBUGME
    printf("Attack! %s\n", mAttackType ? "TELEPORT" : "PROJECTILE");
#endif
    //    SetState(STATE_TELEPORT, mDirection);
    auto *attackSet = mCurrentHealthBar == 4 ? phaseOneAttacks :
                      mCurrentHealthBar == 3 ? phaseTwoAttacks :
                                               phaseThreeAttacks;
    if (attackSet[mAttackIndex] < 0) {
      mAttackIndex = 0;
    }
    switch (attackSet[mAttackIndex]) {
      case CHARGE_ATTACK:
        SetState(STATE_CHARGE, mDirection);
        break;
      case LEAP_ATTACK:
        SetState(STATE_LEAP, mDirection);
        break;
      case PROJECTILE_ATTACK:
        SetState(STATE_PROJECTILE, mDirection);
        break;
      case PILLAR_ATTACK:
        SetState(STATE_PILLAR, mDirection);
        break;
      default:
        Panic("Invalid attack %d", attackSet[mAttackIndex]);
    }
    mAttackIndex++;

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
  if (!mShieldProcess) {
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
  if (MaybeHit()) {
    return ETrue;
  }
  if (!mShieldProcess) {
    SetState(STATE_RESET_SHIELD, DIRECTION_DOWN);
    return ETrue;
  }
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
      gSoundPlayer.TriggerSfx(SFX_MIDBOSS_BOUNCE_WALL_WAV, 4);
      LowerShield();
    }
    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      SetState(STATE_IDLE, mSprite->mDirection);
    }
  } else if (mStateTimer < HOP_DURATION) {
    mStateTimer++;
    mSprite->mDy = GRAVITY * .5 * mStateTimer * (mStateTimer - HOP_DURATION);
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_LAND_WAV, 4);
    MaybeBounce();
  } else if (mStateTimer == HOP_DURATION) {
    Land(mSprite->mDirection);
    mStateTimer++;
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_LAND_WAV, 4);
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

TBool GFinalBossProcess::LeapState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mStateTimer < 0) {
    TFloat xx = GPlayer::mSprite->Center().x - mSprite->Center().x,
      yy = GPlayer::mSprite->Center().y - mSprite->Center().y;
    mDirection = GAnchorSprite::VectorToDirection(xx, yy);
    Leap(mDirection);
    mStateTimer++;
  } else if (mStateTimer == 0) {
    if (mSprite->AnimDone()) {
      TFloat xx = GPlayer::mSprite->Center().x - mSprite->Center().x,
        yy = GPlayer::mSprite->Center().y - mSprite->Center().y;
      mSprite->vx = xx / LEAP_DURATION;
      mSprite->vy = yy / LEAP_DURATION;
      mSprite->ClearFlags(SFLAG_CHECK);
      LowerShield();
      mStateTimer++;
    }
  } else if (mStateTimer < LEAP_DURATION) {
    mSprite->mDy = GRAVITY * TFloat(mStateTimer) * (TFloat(mStateTimer) - LEAP_DURATION);
    if (!mSprite->CanWalk(mSprite->vx, mSprite->vy, true)) {
      mSprite->vx = mSprite->vy = 0;
    }
    mStateTimer++;
  } else if (mStateTimer == LEAP_DURATION) {
    mSprite->mDy = 0;
    mSprite->SetFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    Land(mDirection);
    mStateTimer++;
  } else {
    if (mSprite->AnimDone()) {
      SetState(STATE_IDLE, DIRECTION_DOWN);
    }
  }

  return ETrue;
}

TBool GFinalBossProcess::ProjectileState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (mSprite->AnimDone()) {
    // fire!
    TFloat xx = mSprite->x + 48,
           yy = mSprite->y;
    // fire 10 projectiles in a circle pattern with the boss in the center
    for (TInt16 angle = 0; angle < 360; angle += 360 / 10) {
      mGameState->AddProcess(new GFinalBossProjectileProcess(mGameState, xx, yy, angle, mSprite->mElement));
    }
    SetState(STATE_IDLE, mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::PillarState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mStateTimer == 0) {
    if (mSprite->AnimDone()) {
      mStateTimer++;
    }
    return ETrue;
  }

  // spawn pillars
  for (TInt n = 0; n < 8; n++) {
    // Follows Player if water or fire
    if (mSprite->mElement == ELEMENT_WATER || mSprite->mElement == ELEMENT_FIRE) {
      mGameState->AddProcess(
        new GFinalBossPillarProcess(mGameState, GPlayer::mSprite->mLastX, GPlayer::mSprite->mLastY, mSprite->mElement, ETrue,
                                    (n * 30)));
    } else {
      TPoint p = mSprite->Center();
      p.Offset(Random(-64, 64), Random(-64, 64));

      mGameState->AddProcess(new GFinalBossPillarProcess(mGameState, p.x, p.y, mSprite->mElement, EFalse, 0));
    }
  }
  SetState(STATE_IDLE, mDirection);

  return ETrue;
}

TBool GFinalBossProcess::StunState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mStateTimer < 0) {
    mStateTimer++;
  } else if (mStateTimer == 0) {
    mSprite->StartAnimation(recoverAnimation);
    mStateTimer++;
  } else if (mStateTimer == 1) {
    if (mSprite->AnimDone()) {
      SetState(STATE_RESET_SHIELD, DIRECTION_DOWN);
    }
  }

  return ETrue;
}

TBool GFinalBossProcess::ResetShieldState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    RaiseShield();
    SetState(STATE_IDLE, DIRECTION_DOWN);
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
    case STATE_LEAP:
      return LeapState();
    case STATE_PROJECTILE:
      return ProjectileState();
    case STATE_PILLAR:
      return PillarState();
    case STATE_STUN:
      return StunState();
    case STATE_RESET_SHIELD:
      return ResetShieldState();
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
