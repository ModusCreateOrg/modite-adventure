#include "GFinalBossProcess.h"
#include "GWizardProjectileProcess.h"
#include "GFinalBossDeathProcess.h"
#include "common/GSpellOverlayProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"

enum {
  STATE_IDLE,
  STATE_WALK,
  STATE_FIRE,
  STATE_HIT,
  STATE_SPELL,
  STATE_DEATH,
};

const TFloat WALK_VELOCITY = 1.4;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;

const TInt16 WALK_SPEED = 10;
const TInt16 FIRE_SPEED = 30;
const TInt16 HIT_SPEED = 5;
const TInt16 IDLE_SPEED = 1;
const TInt16 ATTACK_SPEED = 5;


static DIRECTION reverse_direction(DIRECTION d) {
  return (DIRECTION)Random(0,3);
//  switch (d) {
//    case DIRECTION_UP:
//      return DIRECTION_DOWN;
//    default:
//    case DIRECTION_DOWN:
//      return DIRECTION_UP;
//    case DIRECTION_LEFT:
//      return DIRECTION_RIGHT;
//    case DIRECTION_RIGHT:
//      return DIRECTION_LEFT;
//  }
}

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

static ANIMSCRIPT fireUpAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
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
  AFLIP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT fireLeftAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT fireRightAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_DOWN + 0),
  AFLIP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_UP + 0),
  ASTEP(FIRE_SPEED, IMG_FINAL_BOSS_WALK_RIGHT + 0),
  AEND,
};

// constructor
GFinalBossProcess::GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt16 aParams)
    : GProcess(aParams) {
  mGameState = aGameState;
  mSprite = new GAnchorSprite(mGameState, 0, aSlot);
  mSprite->x = aX;
  mSprite->y = aY;
  mStep = 0;
  mSprite->StartAnimation(idleDownAnimation);
  mSprite->type = STYPE_ENEMY;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
//  mSprite->SetFlags(SFLAG_RENDER_SHADOW | SFLAG_KNOCKBACK | SFLAG_CHECK);
  mSprite->SetFlags(SFLAG_KNOCKBACK | SFLAG_CHECK);
  mGameState->AddSprite(mSprite);
  mHitTimer = HIT_SPAM_TIME;
  SetAttackTimer();
  SetState(STATE_IDLE, DIRECTION_DOWN);
};

// destructor
GFinalBossProcess::~GFinalBossProcess() {
  printf("GFinalBoss died!\n ");
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GFinalBossProcess::Idle(DIRECTION aDirection) {
  aDirection = aDirection;
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(idleDownAnimation);
  mStateTimer = 2 * 60;
}

void GFinalBossProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  switch (aDirection) {
    case DIRECTION_UP:
      mDirection = DIRECTION_UP;
      mSprite->vy = -WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkUpAnimation2 : walkUpAnimation1);
      break;
    default:
    case DIRECTION_DOWN:
      mDirection = DIRECTION_DOWN;
      mSprite->vy = WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation2 : walkDownAnimation1);
      break;
    case DIRECTION_LEFT:
      mDirection = DIRECTION_LEFT;
      mSprite->vx = -WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkLeftAnimation2 : walkLeftAnimation1);
      break;
    case DIRECTION_RIGHT:
      mDirection = DIRECTION_RIGHT;
      mSprite->vx = WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation2 : walkRightAnimation1);
      break;
  }
}

void GFinalBossProcess::Fire(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mDirection = aDirection;
  switch (aDirection) {
    case 0:
      printf("earthProjectileAnimation\n");
      mSprite->StartAnimation(earthProjectileAnimation);
      break;
    case 1:
      printf("waterProjectileAnimation\n");

      mSprite->StartAnimation(waterProjectileAnimation);
      break;
    case 2:
      printf("fireProjectileAnimation\n");
      mSprite->StartAnimation(fireProjectileAnimation);
      break;
    case 3:
      printf("energyProjectileAnimation\n");
      mSprite->StartAnimation(energyProjectileAnimation);
      break;
    default:
      printf("DEFAULT energyProjectileAnimation\n");

      mSprite->StartAnimation(earthProjectileAnimation);
      break;
  }
}

void GFinalBossProcess::Hit(DIRECTION aDirection) {
  mDirection = aDirection;
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
}

void GFinalBossProcess::Spell(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
  mSpellCounter += 2;
  auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1);
  //  mSpellOverlayProcess = p;
  mGameState->AddProcess(p);
  p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 44, mSprite->y + 1);
  mGameState->AddProcess(p);
}

void GFinalBossProcess::Death(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(deathAnimation);
  // get coordinates for explosion placement
  TRect r;
  mSprite->GetRect(r);
  //  r.Dump();
  mDeathCounter = 10;
  for (TInt delay = 0; delay < mDeathCounter; delay++) {
    printf("DEATH SPRITE @ %d,%d\n", r.x1, r.x2);
    auto *p = new GFinalBossDeathProcess(mGameState, this, r.x1, r.y1 - 48, delay);
    mGameState->AddProcess(p);
  }
}

void GFinalBossProcess::SetState(TInt aNewState, DIRECTION aNewDirection) {
  mStep = 0;
  mState = aNewState;
  mDirection = aNewDirection;
  switch (aNewState) {
    case STATE_IDLE:
      Idle(mDirection);
      break;
    case STATE_WALK:
      Walk(mDirection);
      break;
    case STATE_FIRE:
      Fire(mDirection);
      break;
    case STATE_HIT:
      Hit(mDirection);
      break;
    case STATE_SPELL:
      Spell(mDirection);
      break;
    case STATE_DEATH:
      Death(mDirection);
      break;
  }
}

TBool GFinalBossProcess::MaybeHit() {
  if (mSprite->TestCType(STYPE_SPELL)) {
    mSprite->ClearCType(STYPE_SPELL);
    if (!mSprite->mInvulnerable) {
      mSprite->mInvulnerable = ETrue;
      // TODO take into account which spellbook is being wielded
      // random variation from 100% to 150% base damage
      TInt hitAmount = GPlayer::mHitStrength + round(RandomFloat() * GPlayer::mHitStrength / 2);
      mSprite->mHitPoints -= hitAmount;
      auto *p = new GStatProcess(mSprite->x + 80, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_ENEMY_HIT);
      mGameState->AddProcess(p);
      if (mSprite->mHitPoints <= 0) {
        printf("FINAL BOSS DEATH\n");
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      SetState(STATE_SPELL, mSprite->mDirection);
      return ETrue;
    }
  }

  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      // random variation from 100% to 150% base damage
      TInt hitAmount = GPlayer::mHitStrength + round(RandomFloat() * GPlayer::mHitStrength / 2);
      mSprite->mHitPoints -= hitAmount;
      auto *p = new GStatProcess(mSprite->x + 80, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_ENEMY_HIT);
      mGameState->AddProcess(p);
      if (mSprite->mHitPoints <= 0) {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      switch (other->mDirection) {
        case DIRECTION_RIGHT:
          SetState(STATE_HIT, DIRECTION_LEFT);
          break;
        case DIRECTION_LEFT:
          SetState(STATE_HIT, DIRECTION_RIGHT);
          break;
        case DIRECTION_UP:
          SetState(STATE_HIT, DIRECTION_DOWN);
          break;
        case DIRECTION_DOWN:
          SetState(STATE_HIT, DIRECTION_UP);
          break;
        default:
          Panic("GWizardProcess no hit direction\n");
          break;
      }
      return ETrue;
    }
  }

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
    return ETrue;
  }

  return EFalse;
}

TBool GFinalBossProcess::MaybeFire() {
  if (--mAttackTimer < 1) {
    SetState(STATE_FIRE, mDirection);
    return ETrue;
  }
  return EFalse;
}

TBool GFinalBossProcess::IdleState() {
  if (MaybeFire()) {
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
  if (MaybeFire()) {
    return ETrue;
  }

  if (!mSprite->CanWalk(mDirection, mSprite->vx, mSprite->vy)) {
    SetState(STATE_WALK, (DIRECTION)Random(0,3));
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    mStep = 1 - mStep;
    Walk(mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::FireState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (mSprite->AnimDone()) {
    // fire!
    TInt16 type = 0;
    switch (mDirection) {
      case 0:
        type = ATTR_WIZARD_EARTH;
        break;
      case 1:
        type = ATTR_WIZARD_WATER;
        break;
      case 2:
        type = ATTR_WIZARD_FIRE;
        break;
      case 3:
        type = ATTR_WIZARD_ENERGY;
        break;

      default:
        type = ATTR_WIZARD_EARTH;
        break;
    }
    TFloat xx = mSprite->x + 48,
           yy = mSprite->y;
    // fire 10 projectiles in a circle pattern with the boss in the center
    for (TFloat angle = 0; angle < 360; angle += 360 / 10) {
      mGameState->AddProcess(new GWizardProjectileProcess(mGameState, xx, yy, angle, type));
    }
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::HitState() {
  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
  }

  if (mHitTimer < 0) {
    mHitTimer = HIT_SPAM_TIME;
    mSprite->mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    // TODO @michaeltintiuc fix this
//    SetState(STATE_TELEPORT, DIRECTION_DOWN);
  }

  if (mSprite->AnimDone()) {
    if (mSprite->mHitPoints <= 0) {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      SetState(STATE_IDLE, mSprite->mDirection);
    }
  }

  return ETrue;
}

TBool GFinalBossProcess::SpellState() {
  if (mSprite->AnimDone() && mSpellCounter <= 0) {
    if (mSprite->mHitPoints <= 0) {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
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
    case STATE_IDLE:
      return IdleState();
    case STATE_WALK:
      return WalkState();
    case STATE_FIRE:
      return FireState();
    case STATE_HIT:
      return HitState();
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
  return ETrue;
}
