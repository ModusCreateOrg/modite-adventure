#include "GEnemyProcess.h"
#include "GGameState.h"

GEnemyProcess::GEnemyProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TUint16 aSlot) :
  mGameState(aGameState), mPlayfield(aGamePlayfield) {
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

  mPlayerSprite = mGameState->PlayerSprite();
  mAttackTimer = Random(30, 60);
}

GEnemyProcess::~GEnemyProcess() {
  mGameState->RemoveSprite(mSprite);
  delete mSprite;
}
