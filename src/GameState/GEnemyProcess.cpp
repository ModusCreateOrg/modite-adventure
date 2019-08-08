#include "GEnemyProcess.h"
#include "GGameState.h"

GEnemyProcess::GEnemyProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TUint16 aSlot, TUint16 aPalette,
                             TUint16 aColors) : mGameState(aGameState), mGamePlayfield(aGamePlayfield) {
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  bm->Remap(aPalette, aColors);
  TRGB *pal = bm->GetPalette();
  for (TInt i = aPalette; i < aPalette + aColors - 1; i++) {
    gDisplay.SetColor(i, pal[i]);
  }
  mState = IDLE_STATE;
  mSprite = new GAnchorSprite(0, aSlot);
  mSprite->type = STYPE_ENEMY;
  mSprite->cMask = STYPE_PLAYER|STYPE_PBULLET;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->w = 32;
  mSprite->h = 32;
  mGameState->AddSprite(mSprite);
  mDirection = DIRECTION_DOWN;
  mState = IDLE_STATE;
  mStep = 0;
}

GEnemyProcess::~GEnemyProcess() {
  delete mSprite;
}

