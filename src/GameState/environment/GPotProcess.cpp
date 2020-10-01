#include "GPotProcess.h"

static const TInt BREAK_SPEED = 5;

static ANIMSCRIPT crateAnimation[] = {
  ALABEL,
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(1000, IMG_POT + 0),
  ALOOP,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ADELTA(1, 1),
  ASTEP(BREAK_SPEED, IMG_POT + 0),
  ADELTA(0, 0),
  ASTEP(BREAK_SPEED, IMG_POT + 0),
  ADELTA(-1, -1),
  AEND,
};

static ANIMSCRIPT breakAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
//  ASTEP(BREAK_SPEED, IMG_POT + 0),
  ASTEP(BREAK_SPEED, IMG_POT + 1),
  ASTEP(BREAK_SPEED, IMG_POT + 2),
  ASTEP(BREAK_SPEED, IMG_POT + 3),
  ASTEP(BREAK_SPEED, IMG_POT + 4),
  ASTEP(BREAK_SPEED, IMG_POT + 5),
  AEND,
};

GPotProcess::GPotProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mAnimating = EFalse;
  mAttribute = ATTR_POT;
  mSprite = new GAnchorSprite(mGameState, POT_PRIORITY, ENVIRONMENT_SLOT, IMG_POT, STYPE_OBJECT);
  mSprite->Name("ENVIRONMENT POT");
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_ENEMY);
  mSprite->w = 32;
  mSprite->h = 24;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 32;
  mHitPoints = 1;
  mSprite->StartAnimation(crateAnimation);
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite);
}

GPotProcess::~GPotProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPotProcess::RunBefore() {
  return ETrue;
}

TBool GPotProcess::RunAfter() {
  if (mAnimating) {
    if (mSprite->AnimDone()) {
      mAnimating = EFalse;
      mSprite->cType = 0;
      if (mHitPoints <= 0) {
        GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y);
        mGameState->EndProgram(mIp, ATTR_POT_GONE, mParam); // do not persist crate, but persist item
        printf("Pot broken param = %x %d\n", mParam, mParam);
        return EFalse;
      }
      mSprite->StartAnimation(crateAnimation);
    }
  }
  else if (mSprite->cType & STYPE_PBULLET) {
    mAnimating = ETrue;
    if (--mHitPoints > 0) {
      mSprite->StartAnimation(hitAnimation);
      gSoundPlayer.TriggerSfx(SFX_POT_TAKE_DAMAGE_WAV);
    }
    else {
      mSprite->StartAnimation(breakAnimation);
      gSoundPlayer.TriggerSfx(SFX_POT_DESTROYED_WAV);
    }
  }
  mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY);

  return ETrue;
}
