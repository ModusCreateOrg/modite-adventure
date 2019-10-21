#include "GFloorSwitchProcess.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

GFloorSwitchProcess::GFloorSwitchProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden)
    : GEnvironmentProcess(aGameState, aParam, aX, aY) {
  mSprite = ENull;
  mImage = IMG_FLOOR_SWITCH + (aWooden ? 2 : 0);

  printf("FLOOR SWITCH %d %0x\n", mParam);
  mSprite = new GAnchorSprite(mGameState, FLOOR_SWITCH_PRIORITY, ENVIRONMENT_SLOT, mImage, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 3;
  mGameState->AddSprite(mSprite);
  mState = mAnimating = EFalse;
  Listen(MESSAGE_FLOOR_SWITCH_DOWN);
  Listen(MESSAGE_FLOOR_SWITCH_UP);
}

GFloorSwitchProcess::~GFloorSwitchProcess() {
  //
}

TBool GFloorSwitchProcess::RunBefore() {
  while (BEventMessage *m = GetMessage()) {
    //    printf("me: %p - Received %d from %p %0x\n", this, m->mType, m->mSender, m->mMessage);
    delete m;
  }

  GGamePlayfield *p = mGameState->mGamePlayfield;

  TInt group = mAttribute->group;
  if (group) {
    if (mState) {
      if (mAttribute->order != OA_ORDER_ANY) {
        p->mGroupState[group] = EFalse;
      }
    }
    else {
      p->mGroupState[group] = EFalse;
      p->mGroupDone[group] = EFalse;
    }
  }

  return ETrue;
}

TBool GFloorSwitchProcess::RunAfter() {
  if (mAnimating) {
    if (mSprite->AnimDone()) {
      mSprite->cType = 0;
      mSprite->SetFlags(SFLAG_CHECK);
      return ETrue;
    }
  }

  if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
    mSprite->ClearFlags(SFLAG_CHECK);
    mState = !mState;
    OBJECT_ATTRIBUTE *oa = (OBJECT_ATTRIBUTE *)&mParam;
    printf("Toggle Floor Switch %s group:%d, order:%d\n", mState ? "ON" : "OFF", oa->group, oa->order);
    mAnimating = ETrue;
    mSprite->mImageNumber = mState ? (mImage + 1) : mImage;
    if (mState) {
      gEventEmitter.FireEvent(this, MESSAGE_FLOOR_SWITCH_DOWN, (TAny *)TUint64(mParam));
    }
    else {
      gEventEmitter.FireEvent(this, MESSAGE_FLOOR_SWITCH_UP, (TAny *)TUint64(mParam));
    }
  }

  return ETrue;
}
