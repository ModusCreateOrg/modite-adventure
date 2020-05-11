#ifndef MODITE_GSPIDERPROCESS_H
#define MODITE_GSPIDERPROCESS_H

#include "GGruntProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSpiderProcess : public GGruntProcess {
public:
  GSpiderProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GSpiderProcess() OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Spell(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif //MODITE_GSPIDERPROCESS_H
