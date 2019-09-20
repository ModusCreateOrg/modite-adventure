#include "GStairsProcess.h"

GStairsProcess::GStairsProcess(GGameState *aGameState, DIRECTION aDirection, TInt aLevel, TFloat aX, TFloat aY)
  : BProcess(0) {
  mSprite1   = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel     = aLevel;
  if (mDirection == DIRECTION_UP) {
    mSprite1 = new GAnchorSprite(999, TILESET_SLOT, IMG_OBJ_STAIRS_UP2);
    mSprite1->x = aX;
    mSprite1->y = aY + 64;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->w = mSprite1->h = 32;
    mSprite1->type = STYPE_OBJECT;
    mSprite1->cMask = STYPE_PLAYER;
    mGameState->AddSprite(mSprite1);

    mSprite2 = new GAnchorSprite(999, TILESET_SLOT, IMG_OBJ_STAIRS_UP1);
    mSprite2->x = aX;
    mSprite2->y = aY + 32;
    mSprite2->w = mSprite2->h = 32;
    mGameState->AddSprite(mSprite2);
  } else {
    mSprite1 = new GAnchorSprite(999, TILESET_SLOT, IMG_OBJ_STAIRS_DOWN);
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

TBool GStairsProcess::RunBefore() {
  return ETrue;
}

TBool GStairsProcess::RunAfter() {
  if (mSprite1->cType) {
    switch (mLevel) {
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
    mSprite1->mCollided->cType &= ~STYPE_OBJECT;
    mSprite1->cType  = 0;
  }
  return ETrue;
}
