#include "GOverworldLevelTransitionProcess.h"

GOverworldLevelTransitionProcess::GOverworldLevelTransitionProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aParams, TFloat aX,
                               TFloat aY, const char *aKind)
    : GEnvironmentProcess(aGameState, aIp, aParams, aX, aY) {
  mSprite = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel = aParams;
//  TBool isWood = strcmp(aKind, "WOOD") == 0;

//  const int img = isWood ? IMG_STONE_STAIRS_DOWN : IMG_WOOD_STAIRS_DOWN;
  mAttribute = ATTR_DUNGEON_ENTRANCE;
  mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, IMG_STONE_STAIRS_DOWN, STYPE_OBJECT);
  mSprite->Name("OVERWORLD LEVEL ENTRANCE");

  TFloat totalMapHeight = aGameState->mGamePlayfield->GetMapHeight() * TILESIZE,
  totalMapWidth  = aGameState->mGamePlayfield->GetMapWidth() * TILESIZE;

  // TODO: @jgarcia @jaygarcia Detect where the entrances are located on the screen and
  // Create the hitbox so it properly is configured to allow the
  // player to move nearly all the way into the tile before making the transition to the next
  // overworld level
  // Dynamic Hitbox Creation
  if (aY == 0 && aX > 0) {
    // Top-most entrance (heading north)
    mSprite->h = 4;
    mSprite->w = 32;
    mSprite->cx = -16;
    mSprite->cy = -28;
  }
  else if (aX == 0 && aY > 0) {
    // Left-most entrance (heading west)
    mSprite->h = 32;
    mSprite->w = 4;
    mSprite->cx = -4;
    mSprite->cy = 0;
  }
  else if (aY == totalMapHeight - TILESIZE) {
    // Bottom-most entrance (heading west)
    mSprite->h = 4;
    mSprite->w = 32;
    mSprite->cx = -16;
    mSprite->cy = 0;
  }
  else if (aX == totalMapWidth - TILESIZE) {
    // Right-most entrance (heading west)
    mSprite->h = 32;
    mSprite->w = 4;
    mSprite->cx = 26;
    mSprite->cy = 0;
  }
  else {
    // catch all
    mSprite->h = 1;
    mSprite->w = 1;
    mSprite->cx = 12;
    mSprite->cy = -12;
  }

  mSprite->x = aX;
  mSprite->y = aY + 32;
  mSprite->SetFlags(SFLAG_BELOW);
  mSprite->cMask = STYPE_PLAYER;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite);
  mSprite->ClearFlags(SFLAG_RENDER);
}

GOverworldLevelTransitionProcess::~GOverworldLevelTransitionProcess() {
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

TBool GOverworldLevelTransitionProcess::RunBefore() {
  return ETrue;
}

TBool GOverworldLevelTransitionProcess::RunAfter() {
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
    const TInt16 dungeon = TUint16(mLevel >> 8) & 0xff,
        level = TUint16(mLevel & 0xff);

    if (level == 10) {
      printf("USE Transition to OVERWOLD\n");
      mGameState->NextLevel(OVERWORLD_DUNGEON, mGameState->LastOverworldLevel());
    }
    else {
      printf("USE STAIRS to dungeon %d level %d\n", dungeon, level);
      mGameState->NextLevel(dungeon == 0 ? -1 : dungeon,level);
    }
    mSprite->mCollided->ClearCType(STYPE_OBJECT);
  }
  mSprite->cType = 0;
  return ETrue;
}
