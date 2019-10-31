#include "GStairsProcess.h"

GStairsProcess::GStairsProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aParams, TFloat aX, TFloat aY, const char *aKind)
    : GEnvironmentProcess(aGameState, aIp, aParams, aX, aY) {
  mSprite1 = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel = aParams;
  if (mDirection == DIRECTION_UP) {
    const int img = strcmp(aKind, "WOOD") ? IMG_STONE_STAIRS_UP : IMG_WOOD_STAIRS_UP;
    mSprite1 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img);
    mSprite1->cx = -16;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->x = aX;
    mSprite1->y = aY + 64;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->type = STYPE_OBJECT;
    mSprite1->cMask = STYPE_PLAYER;

    mSprite2 = new GAnchorSprite(mGameState, STAIRS_SIDE_PRIORITY, ENVIRONMENT_SLOT, img - 10);
    mSprite2->cx = -16;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->x = aX;
    mSprite2->y = aY + 32;
    mSprite2->w = mSprite2->h = 32;
    mGameState->AddSprite(mSprite2);
  }
  else {
    const int img = strcmp(aKind, "WOOD") ? IMG_STONE_STAIRS_DOWN : IMG_WOOD_STAIRS_DOWN;
    mSprite1 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img);
    mSprite1->cx = -16;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->x = aX;
    mSprite1->y = aY + 32;
    mSprite1->type = STYPE_OBJECT;
    mSprite1->cMask = STYPE_PLAYER;
  }
  mGameState->AddSprite(mSprite1);
}

GStairsProcess::~GStairsProcess() {
  //
}

TBool GStairsProcess::RunBefore() {
  return ETrue;
}

TBool GStairsProcess::RunAfter() {
  if (mSprite1->TestCType(STYPE_PLAYER)) {
    mSprite1->type = STYPE_DEFAULT;
    mSprite1->cType = 0;
    mSprite1->cMask = 0;
    mSprite1->ClearCType(STYPE_PLAYER);
    mSprite1->ClearFlags(SFLAG_CHECK);
    if (mSprite2) {
      mSprite2->type = STYPE_DEFAULT;
      mSprite2->cType = 0;
      mSprite2->cMask = 0;
      mSprite2->ClearFlags(SFLAG_CHECK);
    }
    printf("USE STAIRS to level %d\n", mLevel);
    mGameState->NextLevel(DUNGEON_DEV, mLevel);
    mSprite1->mCollided->ClearCType(STYPE_OBJECT);
  }
  mSprite1->cType = 0;
  return ETrue;
}
