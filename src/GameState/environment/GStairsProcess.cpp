#include "GStairsProcess.h"

GStairsProcess::GStairsProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aParams, TFloat aX, TFloat aY, const char *aKind)
    : GEnvironmentProcess(aGameState, aIp, aParams, aX, aY) {
  mSprite = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel = aParams;
  TBool isWood = strcmp(aKind, "WOOD") == 0;
  if (mDirection == DIRECTION_UP) {
    mAttribute = isWood ? ATTR_WOOD_STAIRS_UP : ATTR_STONE_STAIRS_UP;
    const int img = strcmp(aKind, "WOOD") ? IMG_STONE_STAIRS_UP : IMG_WOOD_STAIRS_UP;
    mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img, STYPE_OBJECT);
    mSprite->Name(isWood ? "ENVIRONMENT WOOD STAIRS UP" : "ENVIRONMENT STONE STAIRS UP");
    mSprite->cx = -16;
    mSprite->w = mSprite->h = 32;
    mSprite->x = aX;
    mSprite->y = aY + 64;
    mSprite->w = mSprite->h = 32;
    mSprite->SetCMask(STYPE_PLAYER | STYPE_ENEMY);
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP_SPRITES);
    mGameState->AddSprite(mSprite);

    mSprite2 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img - 10);
    mSprite->Name(isWood ? "IGNORE WOOD STAIRS UP 2" : "IGNORE STONE STAIRS UP 2");
    mSprite2->SetFlags(SFLAG_BELOW);
    mSprite2->cx = -16;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->x = aX;
    mSprite2->y = aY + 32;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP_SPRITES);
    mGameState->AddSprite(mSprite2);
  }
  else {
    const int img = isWood ? IMG_STONE_STAIRS_DOWN : IMG_WOOD_STAIRS_DOWN;
    mAttribute = isWood ? ATTR_WOOD_STAIRS_DOWN : ATTR_STONE_STAIRS_DOWN;
    mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img, STYPE_OBJECT);
    mSprite->Name(isWood ? "ENVIRONMENT WOOD STAIRS DOWN" : "ENVIRONMENT STONE STAIRS DOWN");
    mSprite->cx = -16;
    mSprite->w = mSprite->h = 32;
    mSprite->x = aX;
    mSprite->y = aY + 32;
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->SetCMask(STYPE_PLAYER | STYPE_ENEMY);
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP_SPRITES);
    mGameState->AddSprite(mSprite);
  }
}

GStairsProcess::~GStairsProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GStairsProcess::RunBefore() {
  return ETrue;
}

TBool GStairsProcess::RunAfter() {
  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->type = STYPE_DEFAULT;
    mSprite->cType = 0;
    mSprite->cMask = 0;
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->ClearFlags(SFLAG_CHECK);
    if (mSprite2) {
      mSprite2->type = STYPE_DEFAULT;
      mSprite2->cType = 0;
      mSprite2->cMask = 0;
      mSprite2->ClearFlags(SFLAG_CHECK);
    }
    printf("USE STAIRS to level %d\n", mLevel);
    mGameState->NextLevel(DUNGEON_DEV, mLevel);
    mSprite->mCollided->ClearCType(STYPE_OBJECT);
  }
  mSprite->cType = 0;
  return ETrue;
}
