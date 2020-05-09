#include "GBossProcess.h"
#include "GPlayer.h"

GBossProcess::GBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute) : GLivingProcess(aGameState, aAttribute) {
  mSprite = new GAnchorSprite(mGameState, 0, aSlot, 0, STYPE_ENEMY);
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  mSprite->SetFlags(SFLAG_KNOCKBACK | SFLAG_CHECK);
  mSprite->x = mStartX = aX;
  mSprite->y = mStartY = aY;
  mDirection = mSprite->mDirection = DIRECTION_DOWN;
  mGameState->AddSprite(mSprite);
  mDeathCounter = 0;
  mSpellCounter = 0;
  GPlayer::mActiveBoss = mSprite;
}

GBossProcess::~GBossProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  GPlayer::mActiveBoss = ENull;
}
