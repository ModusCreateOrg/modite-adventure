/**
  * Final Boss Pillar Process
  */

#include "GFinalBossPillarProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

const TInt16 PILLAR_SPEED = 4;

static ANIMSCRIPT pillarFireAnimation[] = {
  ABITMAP(FIRE_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};
static ANIMSCRIPT pillarEarthAnimation[] = {
  ABITMAP(EARTH_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT pillarWaterAnimation[] = {
  ABITMAP(WATER_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT pillarEnergyAnimation[] = {
  ABITMAP(ENERGY_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};


static ANIMSCRIPT explodeAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 6),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 7),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 8),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 9),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 10),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 11),
  AEND,
};

// constructor
GFinalBossPillarProcess::GFinalBossPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt16 aSlot, TBool aFollowPlayer, TInt aStartDelay = 0)
    : GProcess(0, 0) {
  mGameState = aGameState;
  mSaveToStream = EFalse;
#ifdef DEBUGME
  // can't use mSprite->Name() since sprite hasn't been spawned yet
  printf("Final Boss Piller %p slot %d (earth %d)\n", this, aSlot, EARTH_FINAL_BOSS_PILLAR_SLOT);
#endif
  mSprite = new GAnchorSprite(mGameState, aSlot, 0);
  {
    char buf[128];
    sprintf(buf, "Final Boss Pillar %p", this);
    mSprite->Name(buf);
  }
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 24;
  mSprite->h = 24;
  mSprite->cy = 0;
  mSprite->cx = -8;
  mSprite->vy = mSprite->vx = 0;
  //  mSprite->type = STYPE_EBULLET;
  mFollowPlayer = aFollowPlayer;
  //  mSprite->SetCMask(STYPE_PLAYER);
  //  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->mAttackStrength = 55;
  mGameState->AddSprite(mSprite);
  switch (aSlot) {
    case FIRE_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s FIRE PILLER\n", mSprite->Name());
#endif
      mSprite->StartAnimation(pillarFireAnimation);
      break;
    case EARTH_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s EARTH PILLER\n", mSprite->Name());
#endif
      mSprite->StartAnimation(pillarEarthAnimation);
      break;
    case WATER_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s WATER PILLER\n", mSprite->Name());
#endif
      mSprite->StartAnimation(pillarWaterAnimation);
      break;
    case ENERGY_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s ENERGY PILLER\n", mSprite->Name());
#endif
      mSprite->StartAnimation(pillarEnergyAnimation);
      break;
    default:
      Panic("%s invalid slot %d\n", mSprite->Name(), aSlot);
  }
  mExploding = EFalse;
  mFrame = 0;
  mStartDelay = (aStartDelay > 0) ? (30 + aStartDelay) : 0;
}

GFinalBossPillarProcess::~GFinalBossPillarProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GFinalBossPillarProcess::RunBefore() {
  if (mSprite->Clipped() || (mExploding && mSprite->AnimDone())) {
#ifdef DEBUGME
    printf("%s REJECTED Clipped or Explosion done %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(DIRECTION_UP, 0, -10)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_UP %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(DIRECTION_RIGHT, 32, 0)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_RIGHT %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(DIRECTION_LEFT, -32, 0)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_LEFT %d\n", mSprite->Name(), mFrame);
#endif

    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(DIRECTION_DOWN, 0, 10)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_DOWN %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (mStartDelay > 0) {
    mStartDelay--;

    if (mFollowPlayer) {
      mSprite->x = GPlayer::mSprite->x + 16;
      mSprite->y = GPlayer::mSprite->y;
    }
    return ETrue;
  }

  //  if (mFrame == 0 && !mExploding) {
  //    mSprite->StartAnimation(pillarAnimation);
  //  }

  if (mFrame > 30) {
    mSprite->SetCMask(STYPE_PLAYER);
    mSprite->SetFlags(SFLAG_CHECK);
    mSprite->type = STYPE_EBULLET;
  }

  mFrame++;

  return ETrue;
}

TBool GFinalBossPillarProcess::RunAfter() {
  if (!mExploding) {
    const TUint16 mFrameMax = 20;
    if (mFrame > mFrameMax) {
#ifdef DEBUGME
      printf("%s timed out\n", mSprite->Name());
#endif
      mSprite->StartAnimation(explodeAnimation);
      return mExploding = ETrue;
    }

    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      mSprite->StartAnimation(explodeAnimation);
      mExploding = ETrue;
    }
  }
  return ETrue;
}
