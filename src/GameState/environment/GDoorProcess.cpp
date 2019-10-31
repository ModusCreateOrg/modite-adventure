#include "GDoorProcess.h"
#include "GGamePlayfield.h"

GDoorProcess::GDoorProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY, ENVIRONMENT_PRIORITY_DOOR) {

  mHorizontal = aHorizontal;

  if (mHorizontal) {
    mSprite1 = new GAnchorSprite(
        mGameState,
        DOOR_PRIORITY, ENVIRONMENT_SLOT, aWood ? IMG_WOOD_DOOR_H : IMG_METAL_DOOR_H,
        STYPE_ENEMY);
  }
  else {
    mSprite1 = new GAnchorSprite(
        mGameState,
        DOOR_PRIORITY,
        ENVIRONMENT_SLOT, aWood ? IMG_WOOD_DOOR_V : IMG_METAL_DOOR_V,
        STYPE_ENEMY);
  }

  mSprite1->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
  mSprite1->w = mHorizontal ? 32 : 16;
  mSprite1->h = mHorizontal ? 16 : 64;
  mSprite1->cx = mHorizontal ? -16 : 0;
  mSprite1->cy = mHorizontal ? 0 : 8;
  mSprite1->x = aX;
  mSprite1->y = aY + (mHorizontal ? 2 : 3);
  mGameState->AddSprite(mSprite1);

  if (!mHorizontal) {
    mSprite2 = new GAnchorSprite(
        mGameState,
        DOOR_PRIORITY,
        ENVIRONMENT_SLOT,
        aWood ? IMG_WOOD_DOOR_V - 10 : IMG_METAL_DOOR_V - 10,
        STYPE_ENEMY);

    mSprite2->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
    mSprite2->w = 16;
    mSprite2->h = 64;
    mSprite2->cx = 0;
    mSprite2->cy = 40;
    mSprite2->x = mSprite1->x;
    mSprite2->y = mSprite1->y - 32;
    mGameState->AddSprite(mSprite2);
  }
}

GDoorProcess::~GDoorProcess() {
  //
}

TBool GDoorProcess::RunBefore() {
  // if door is in a group (realted to switches, etc.), open when the group is "done"
  // "done" means player has successfully thrown the switchs in the right order.
  TInt group = mAttribute->group;
  if (group && mGameState->mGamePlayfield->mGroupDone[group]) {
    // open door
    mGameState->EndProgram(mIp);
    return EFalse;
  }
  return ETrue;
}

TBool GDoorProcess::RunAfter() {
  // if is in a group, we don't open on collisions.
  if (mAttribute->group) {
    return ETrue;
  }
  if (mSprite2) {
    TInt cType1 = mSprite1->cType;
    TInt cType2 = mSprite2->cType;
    if (mSprite1->TestCType(STYPE_PBULLET) && mSprite2->TestCType(STYPE_PBULLET)) {
      mGameState->EndProgram(mIp);
      return EFalse;
    }
  }
  else {
    if (mSprite1->TestCType(STYPE_PBULLET)) {
      return EFalse;
    }
  }
  return ETrue;
}
