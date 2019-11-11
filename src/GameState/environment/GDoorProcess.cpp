#include "GDoorProcess.h"
#include "GGamePlayfield.h"
#include "BSpriteSheet.h"

GDoorProcess::GDoorProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood,
                           TBool aHorizontal)
  : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY, ENVIRONMENT_PRIORITY_DOOR) {

  mHorizontal = aHorizontal;

  if (mHorizontal) {
    mSprite1 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_H : IMG_METAL_DOOR_H,
                                 STYPE_ENEMY);
    mSprite1->Name(aWood ? "HORIZONTAL WOOD DOOR 1" : "HORIZONTAL METAL DOOR 1");
  } else {
    mSprite1 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_V : IMG_METAL_DOOR_V,
                                 STYPE_ENEMY);
    mSprite1->Name(aWood ? "VERTICAL WOOD DOOR 1" : "VERTICAL METAL DOOR 1");
//    mSprite1->SetFlags(SFLAG_COLLIDE2D);
  }

  mSprite1->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
  mSprite1->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP_SPRITES);

  mSprite1->w = 32;
  mSprite1->h = (mHorizontal ? 32 : 64);
  mSprite1->cx = -16;
  mSprite1->x = aX;
  mSprite1->y = aY + (mHorizontal ? 2.0 : 3.0);
  mGameState->AddSprite(mSprite1);
  if (!mHorizontal) {
    mSprite1->SetWall(ETrue);
  }

  if (!mHorizontal) {
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_V - 10 : IMG_METAL_DOOR_V - 10, STYPE_ENEMY);

    mSprite2->Name(aWood ? "VERTICAL WOOD DOOR 2" : "VERTICAL METAL DOOR 2");
    mSprite2->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
//    mSprite2->SetFlags(SFLAG_COLLIDE2D);

    mSprite2->w = 32;
    mSprite2->h = 64;
    mSprite2->cx = -16;
    mSprite2->cy = 32;
    mSprite2->x = mSprite1->x;
    mSprite2->y = mSprite1->y - 32;
    mSprite2->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP_SPRITES);
    mGameState->AddSprite(mSprite2);
    if (!mHorizontal) {
      mSprite2->SetWall(ETrue);
    }
  }
}

GDoorProcess::~GDoorProcess() {
  ClearWall();
  if (mSprite1) {
    mSprite1->Remove();
    delete mSprite1;
    mSprite1 = ENull;
  }
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }
}

void GDoorProcess::ClearWall() {
  mSprite1->SetWall(EFalse);
  if (mSprite2) {
    mSprite2->SetWall(EFalse);
  }
}

TBool GDoorProcess::RunBefore() {
  // if door is in a group (realted to switches, etc.), open when the group is "done"
  // "done" means player has successfully thrown the switchs in the right order.
  TInt group = mAttribute->group;
  if (group && mGameState->mGamePlayfield->mGroupDone[group]) {
    // open door
    ClearWall();
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

  if (mSprite1->TestCType(STYPE_PBULLET) || (mSprite2 && mSprite2->TestCType(STYPE_PBULLET))) {
    ClearWall();
    mGameState->EndProgram(mIp);
    return EFalse;
  }
  mSprite1->cType = 0;
  if (mSprite2) {
    mSprite2->cType = 0;
  }

  return ETrue;
}
