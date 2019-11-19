#include "GEnemyDeathOverlayProcess.h"
#include "GGameState.h"

const TInt16 DEATH_SPEED = 4;

// ANIMATIONS

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(ENEMY_DEATH_SLOT),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 0),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 1),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 2),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 3),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 4),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 5),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 6),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 7),
  ANULL(3),
  AEND,
};

GEnemyDeathOverlayProcess::GEnemyDeathOverlayProcess(GGameState *aGameState, TFloat aX, TFloat aY) 
: GProcess(ATTR_GONE) {
  mSprite = new GAnchorSprite(aGameState, ENEMY_DEATH_PRIORITY, ENEMY_DEATH_SLOT);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->Name("ENEMY DEATH SPRITE");
  mSprite->ClearFlags(SFLAG_CHECK);
  aGameState->AddSprite(mSprite);
  mSprite->StartAnimation(deathAnimation);
}

GEnemyDeathOverlayProcess::~GEnemyDeathOverlayProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyDeathOverlayProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyDeathOverlayProcess::RunAfter() {
  if (mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}
