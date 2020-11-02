/**
  * Final Boss Pillar Process
  */

#include "GFinalBossPillarProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

// constructor
GFinalBossPillarProcess::GFinalBossPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY, ELEMENT aElement, TBool aFollowPlayer, TInt aStartDelay = 0)
    : GProcess(0, 0) {
  mGameState = aGameState;
  mSaveToStream = EFalse;
#ifdef DEBUGME
  // can't use mSprite->Name() since sprite hasn't been spawned yet
  printf("Final Boss Piller %p slot %d (earth %d)\n", this, aSlot, EARTH_FINAL_BOSS_PILLAR_SLOT);
#endif
  mSprite = new GFinalBossPillarSprite(aGameState, aX, aY, aElement);
  {
    char buf[128];
    sprintf(buf, "Final Boss Pillar %p", this);
    mSprite->Name(buf);
  }
  //  mSprite->type = STYPE_EBULLET;
  mFollowPlayer = aFollowPlayer;
  //  mSprite->SetCMask(STYPE_PLAYER);
  //  mSprite->SetFlags(SFLAG_CHECK);
  mGameState->AddSprite(mSprite);
  mExploding = EFalse;
  mFrame = 0;
  mStartDelay = (aStartDelay > 0) ? (30 + aStartDelay) : 0;
  gSoundPlayer.TriggerSfx(SFX_MIDBOSS_ATTACK_FIRE_WAV, 3);
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

  if (!mFollowPlayer && !mSprite->CanWalk(0, -10)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_UP %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(32, 0)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_RIGHT %d\n", mSprite->Name(), mFrame);
#endif
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(-32, 0)) {
#ifdef DEBUGME
    printf("%s REJECTED DIRECTION_LEFT %d\n", mSprite->Name(), mFrame);
#endif

    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(0, 10)) {
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
      mSprite->Explode();
      return mExploding = ETrue;
    }

    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      mSprite->Explode();
      mExploding = ETrue;
    }
  }
  return ETrue;
}
