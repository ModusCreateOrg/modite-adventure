#include "GEnemyProcess.h"
#include "GGameState.h"

GEnemyProcess::GEnemyProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TUint16 aSlot) :
  mGameState(aGameState), mPlayfield(aGamePlayfield) {
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  mState  = IDLE_STATE;
  mSprite = new GAnchorSprite(0, aSlot);
  mSprite->type  = STYPE_ENEMY;
  mSprite->cMask = STYPE_PLAYER | STYPE_PBULLET;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->w     = 32;
  mSprite->h     = 32;
  mGameState->AddSprite(mSprite);
  mDirection = DIRECTION_DOWN;
  mState     = IDLE_STATE;
  mStep      = 0;
}

GEnemyProcess::~GEnemyProcess() {
  mGameState->RemoveSprite(mSprite);
  delete mSprite;
}
