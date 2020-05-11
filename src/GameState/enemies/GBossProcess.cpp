#include "GBossProcess.h"
#include "GPlayer.h"

GBossProcess::GBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute) : GEnemyProcess(aGameState, aX, aY, aSlot, aAttribute) {
  mHitTimer = mAttackTimer = mStateTimer = 0;
  mDeathCounter = 0;
  mSpellCounter = 0;
  GPlayer::mActiveBoss = this;
}

GBossProcess::~GBossProcess() {
  GPlayer::mActiveBoss = ENull;
}
