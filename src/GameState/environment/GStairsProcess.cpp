#include "GStairsProcess.h"

GStairsProcess::GStairsProcess(GGameState *aGameState, DIRECTION aDirection, TInt aLevel, TFloat aX, TFloat aY,
                               const char *aKind)
  : BProcess(0) {
  mSprite1 = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel = aLevel;
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
    mGameState->AddSprite(mSprite1);

    mSprite2 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img - 10);
    mSprite2->cx = -16;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->x = aX;
    mSprite2->y = aY + 32;
    mSprite2->w = mSprite2->h = 32;
    mGameState->AddSprite(mSprite2);
  } else {
    const int img = strcmp(aKind, "WOOD") ? IMG_STONE_STAIRS_DOWN : IMG_WOOD_STAIRS_DOWN;
    mSprite1 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img);
    mSprite1->cx = -16;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->x = aX;
    mSprite1->y = aY + 32;
    mSprite1->type = STYPE_OBJECT;
    mSprite1->cMask = STYPE_PLAYER;
    mGameState->AddSprite(mSprite1);
  }
}

GStairsProcess::~GStairsProcess() {
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

TBool GStairsProcess::RunBefore() {
  return ETrue;
}

TBool GStairsProcess::RunAfter() {
  if (mSprite1->TestCType(STYPE_PLAYER)) {
    switch (mLevel) {
      case 0:
      case 1:
        mGameState->NextLevel("Dungeon0", mLevel, DEVDUNGEON_0_LEVEL1_MAP);
        break;
      case 2:
        mGameState->NextLevel("Dungeon0", mLevel, DEVDUNGEON_0_LEVEL2_MAP);
        break;
      case 3:
        mGameState->NextLevel("Dungeon0", mLevel, DEVDUNGEON_0_LEVEL3_MAP);
        break;
    }
    mSprite1->mCollided->ClearCType(STYPE_OBJECT);
  }
  mSprite1->cType = 0;
  return ETrue;
}
