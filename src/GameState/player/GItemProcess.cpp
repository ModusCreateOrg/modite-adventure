#include "GItemProcess.h"
#include "GResources.h"
#include "GPlayer.h"
#include "ITems.h"
#include "GStatSprite.h"
#include "GStatProcess.h"

GItemProcess *GItemProcess::SpawnItem(GGameState *aGameState, TInt aItemNumber, TFloat aX, TFloat aY) {
  if (aItemNumber && aItemNumber < sizeof(items)) {
    GItemProcess *p = new GItemProcess(aGameState, aItemNumber, aX, aY);
    aGameState->AddProcess(p);
    return p;
  } else {
    return ENull;
  }
}

GItemProcess::GItemProcess(GGameState *aGameState, TInt aItemNumber, TFloat aX, TFloat aY) : BProcess() {
  mGameState  = aGameState;
  mItemNumber = aItemNumber;
  if (mItemNumber && mItemNumber < sizeof(items)) {
    mSprite = new GAnchorSprite(998, ENVIRONMENT_SLOT, items[mItemNumber]);
    mSprite->type  = STYPE_OBJECT;
    mSprite->cMask = STYPE_PLAYER;
    mSprite->w     = mSprite->h = 32;
    mSprite->cx    = -16;
    mSprite->x     = aX;
    mSprite->y     = aY;
    mGameState->AddSprite(mSprite);
  } else {
    Panic("GItemProcess contructor: invalid item number: %d\n", mItemNumber);
  }
}

GItemProcess::~GItemProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GItemProcess::RunBefore() {
  return ETrue;
}

TBool GItemProcess::RunAfter() {
  if (mSprite->cType & STYPE_PLAYER) {
    GStatProcess *p = new GStatProcess(mSprite->x, mSprite->y, itemNames[mItemNumber]);
    p->SetImageNumber(mSprite->mImageNumber);
    p->SetTimeout(5 * FRAMES_PER_SECOND);
    mGameState->AddProcess(p);
    GPlayer::mInventoryList.PickupItem(mItemNumber);
    GPlayer::mInventoryList.Dump();
    return EFalse;
  }
  return ETrue;
}
