#include "GDoorProcess.h"
#include "GGamePlayfield.h"

GDoorProcess::GDoorProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood,
                           TBool aHorizontal) {
  mGameState = aGameState;
  mParam = aParam;
  mHorizontal = aHorizontal;
  mSprite1 = mSprite2 = ENull;

  if (mHorizontal) {
    mSprite1 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT, aWood ? IMG_WOOD_DOOR_H : IMG_METAL_DOOR_H,
                                 STYPE_ENEMY);
  } else {
    mSprite1 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT, aWood ? IMG_WOOD_DOOR_V : IMG_METAL_DOOR_V,
                                 STYPE_ENEMY);
  }
  mSprite1->cMask = STYPE_PBULLET;
  mSprite1->cMask &= ~STYPE_PLAYER;
  mSprite1->w = mSprite1->h = 32;
  mSprite1->cx = -16;
  mSprite1->x = aX;
  mSprite1->y = aY + 3;
  mGameState->AddSprite(mSprite1);

  if (!mHorizontal) {
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT, aWood ? IMG_WOOD_DOOR_V - 10 : IMG_METAL_DOOR_V - 10,
                                 STYPE_ENEMY);
    mSprite2->cMask = STYPE_PBULLET;
    mSprite2->cMask &= ~STYPE_PLAYER;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->cx = -16;
    mSprite2->x = mSprite1->x;
    mSprite2->y = mSprite1->y - 32;
    mGameState->AddSprite(mSprite2);
  }
}

GDoorProcess::~GDoorProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }
  if (mSprite1) {
    mSprite1->Remove();
    delete mSprite1;
    mSprite1 = ENull;
  }
}

TBool GDoorProcess::RunBefore() {
  return ETrue;
}

TBool GDoorProcess::RunAfter() {
  if ((mSprite1 && (mSprite1->cType & STYPE_PBULLET)) || (mSprite2 && (mSprite2->cType & STYPE_PBULLET))) {
    return EFalse;
  } else {
    return ETrue;
  }
}
