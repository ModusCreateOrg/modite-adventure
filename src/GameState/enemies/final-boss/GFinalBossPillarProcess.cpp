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
  mSprite->ClearFlags(SFLAG_CHECK | SFLAG_RENDER | SFLAG_RENDER_SHADOW);
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
  mStartDelay = aStartDelay;
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
    return EFalse;
  }

  if (!mFollowPlayer && !mSprite->CanWalk(0, 0, ETrue)) {
    return EFalse;
  }

  if (mStartDelay >= 0) {
    if (mStartDelay == 0) {
      mSprite->SetFlags(SFLAG_RENDER | SFLAG_RENDER_SHADOW);
      mSprite->Warn();
      if (mFollowPlayer) {
        mSprite->x = GPlayer::mSprite->x + 16;
        mSprite->y = GPlayer::mSprite->y + 1;
      }
    }
    mStartDelay--;
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    mExploding = ETrue;
    mSprite->Explode();
    mSprite->SetFlags(SFLAG_CHECK);
  }

  if (!mExploding && mFollowPlayer) {
    mSprite->x = ((mSprite->x * 20 * FACTOR) + GPlayer::mSprite->mLastX + 16) / (20 * FACTOR + 1);
    mSprite->y = ((mSprite->y * 20 * FACTOR) + GPlayer::mSprite->mLastY) / (20 * FACTOR + 1);
  }

  return ETrue;
}

TBool GFinalBossPillarProcess::RunAfter() {
  mSprite->ClearCType(STYPE_PLAYER);

  return ETrue;
}
