#ifndef MODITE_GGOBLINSNIPERTURRETPROCESS_H
#define MODITE_GGOBLINSNIPERTURRETPROCESS_H

#include <GameState/enemies/grunts/GGoblinSniperProcess.h>

class GGoblinSniperTurretProcess : public GGoblinSniperProcess {
public:
  GGoblinSniperTurretProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam);
  ~GGoblinSniperTurretProcess() OVERRIDE;

protected:
  TBool IsInRange() OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
};

#endif //MODITE_GGOBLINSNIPERTURRETPROCESS_H
