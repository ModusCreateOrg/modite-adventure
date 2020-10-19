#include "GGoblinSniperTurretProcess.h"

GGoblinSniperTurretProcess::GGoblinSniperTurretProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam)
    : GGoblinSniperProcess(aGameState, aIp, aX, aY, aParam) {
}

GGoblinSniperTurretProcess::~GGoblinSniperTurretProcess() = default;

TBool GGoblinSniperTurretProcess::IsInRange() {
  return ETrue;
}

void GGoblinSniperTurretProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
}

