/**
  * Final Boss Process
  */

#include "GFinalBossProcess.h"
#include "GFinalBossProjectileProcess.h"
#include "GFinalBossPillarProcess.h"
#include "GFinalBossDeathProcess.h"
#include "common/GSpellOverlayProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

enum {
  STATE_IDLE,
  STATE_WALK,
  STATE_PROJECTILE,
  STATE_TELEPORT,
  STATE_HIT,
  STATE_SPELL,
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
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;

const TInt16 WALK_SPEED = 10;
const TInt16 PROJECTILE_SPEED = 30;
const TInt16 HIT_SPEED = 5;
const TInt16 IDLE_SPEED = 1;
const TInt16 ATTACK_SPEED = 5;

static DIRECTION reverse_direction(DIRECTION d) {
  //  return (DIRECTION)Random(0, 3);
  switch (d) {
    case DIRECTION_UP:
      return DIRECTION_DOWN;
    default:
    case DIRECTION_DOWN:
      return DIRECTION_UP;
    case DIRECTION_LEFT:
      return DIRECTION_RIGHT;
    case DIRECTION_RIGHT:
      return DIRECTION_LEFT;
  }
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

// constructor
GFinalBossProcess::GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams)
    : GProcess(aParams) {
  mGameState = aGameState;
  mSprite = new GAnchorSprite(mGameState, 0, BOSS_SLOT);
  mSprite->Name("Final Boss");
  mSprite->x = aX;
  mSprite->y = aY;
  mStep = 0;
  mSprite->StartAnimation(idleDownAnimation);
  mSprite->type = STYPE_ENEMY;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  mSprite->SetFlags(SFLAG_KNOCKBACK | SFLAG_CHECK);
  mGameState->AddSprite(mSprite);
  mHitTimer = HIT_SPAM_TIME;
  mAttackType = EFalse;
  SetAttackTimer();
  SetState(STATE_IDLE, DIRECTION_DOWN);
  GPlayer::mActiveBoss = mSprite;
};

// destructor
GFinalBossProcess::~GFinalBossProcess() {
#ifdef DEBUGME
  printf("GFinalBoss died!\n ");
#endif
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  GPlayer::mActiveBoss = ENull;
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

void GFinalBossProcess::Teleport(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(teleportAnimation1);
  mStep = 0;
  mSprite->mInvulnerable = ETrue;
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
      Idle(mDirection);
      break;
    case STATE_WALK:
      Walk(mDirection);
      break;
    case STATE_PROJECTILE:
      Projectile(mDirection);
      break;
    case STATE_TELEPORT:
      Teleport(mDirection);
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
#ifdef DEBUGME
        printf("FINAL BOSS DEATH\n");
#endif
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
          Panic("GFinalBossProcess no hit direction\n");
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

TBool GFinalBossProcess::MaybeAttack() {
  if (--mAttackTimer < 0) {
#ifdef DEBUGME
    printf("Attack! %s\n", mAttackType ? "TELEPORT" : "PROJECTILE");
#endif
    //    SetState(STATE_TELEPORT, mDirection);
    SetState(mAttackType ? STATE_TELEPORT : STATE_PROJECTILE, mDirection);
    mAttackType = !mAttackType;
    return ETrue;
  }
  return EFalse;
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

  if (!mSprite->CanWalk(mDirection, mSprite->vx, mSprite->vy)) {
#ifdef DEBUGME
    printf("Final Boss can't walk direction %s\n", direction_names[mDirection]);
#endif
    SetState(STATE_WALK, reverse_direction(mDirection));
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    mStep = 1 - mStep;
    Walk(mDirection);
  }
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
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
  }
  return ETrue;
}

TBool GFinalBossProcess::TeleportState() {
  // check to see if boss has met a wall
  TBool canWalkUp = mSprite->CanWalk(DIRECTION_UP, mSprite->vx, mSprite->vy),
        canWalkLt = mSprite->CanWalk(DIRECTION_LEFT, mSprite->vx, mSprite->vy),
        canWalkRt = mSprite->CanWalk(DIRECTION_RIGHT, mSprite->vx, mSprite->vy),
        canWalkDn = mSprite->CanWalk(DIRECTION_DOWN, mSprite->vx, mSprite->vy);

  // Bound around the room while the player is being attacked by the spikes/pillars
  if (!canWalkUp) {
#ifdef DEBUGME
    printf("%s can't walk up\n", mSprite->Name());
#endif
    mSprite->vy *= -1;
  }
  else if (!canWalkDn) {
#ifdef DEBUGME
    printf("%s can't walk down\n", mSprite->Name());
#endif
    mSprite->vy *= -1;
  }

  if (!canWalkLt) {
#ifdef DEBUGME
    printf("%s can't walk left\n", mSprite->Name());
#endif
    mSprite->vx *= -1;
  }
  else if (!canWalkRt) {
#ifdef DEBUGME
    printf("%s can't walk right\n", mSprite->Name());
#endif
    mSprite->vx *= -1;
  }

  if (!mSprite->AnimDone()) {
    return ETrue;
  }

#ifdef DEBUGME
  printf("%s teleport animation done\n", mSprite->Name());
#endif
  if (!mStep) {
#ifdef DEBUGME
    printf("%s start teleport animation 2\n", mSprite->Name());
#endif
    mSprite->StartAnimation(teleportAnimation2);
    mSprite->ClearFlags(SFLAG_RENDER | SFLAG_CHECK); // Let's not hit the player.
    mStep++;

    //    int nTries = 0;
    while (mSprite->vx == 0) {
      //      nTries ++;
      mSprite->vx = (TFloat)Random(-3, 3);
    }
    //    printf("nTries VX = %i\n", nTries);

    //    nTries = 0;
    while (mSprite->vy == 0) {
      //      nTries++;
      mSprite->vy = (TFloat)Random(-3, 3);
    }
    //    printf("nTries VX = %i\n", nTries);

    // spawn pillars
    TInt type = ATTR_FINAL_BOSS_EARTH + Random(0, 3);
    TInt16 slot = EARTH_FINAL_BOSS_PILLAR_SLOT + Random(0, 3);
    for (TInt n = 0; n < 8; n++) {
      // Follows Player if water or fire
      if (type == ATTR_FINAL_BOSS_WATER || type == ATTR_FINAL_BOSS_FIRE) {
        mGameState->AddProcess(new GFinalBossPillarProcess(mGameState, GPlayer::mSprite->mLastX, GPlayer::mSprite->mLastY, slot, ETrue, (n * 30)));
      }
      else {
        TInt pillarX = mSprite->x + Random(-64, 64),
             pillarY = mSprite->y + Random(-64, 64);

        mGameState->AddProcess(new GFinalBossPillarProcess(mGameState, pillarX, pillarY, slot, EFalse, 0));
      }
    }

    return ETrue;
  }
  else if (mStep == 1 && --mStateTimer < 1) {
#ifdef DEBUGME
    printf("%s start teleport animation 3\n", mSprite->Name());
#endif
    mState++;
    mSprite->StartAnimation(teleportAnimation3);
    mSprite->vx = mSprite->vy = 0;
    mSprite->SetFlags(SFLAG_RENDER | SFLAG_CHECK);
  }
  else {
#ifdef DEBUGME
    printf("%s start teleport invisible\n", mSprite->Name());
#endif
    mSprite->SetFlags(SFLAG_RENDER | SFLAG_CHECK);
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
    SetState(STATE_TELEPORT, DIRECTION_DOWN);
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
    case STATE_PROJECTILE:
      return ProjectileState();
    case STATE_TELEPORT:
      return TeleportState();
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
