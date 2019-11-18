#ifndef GMIDBOSSFIREPROCESS_H
#define GMIDBOSSFIREPROCESS_H

#include "GMidBossProcess.h"

class GMidBossGenericProcess : public GMidBossProcess {
public:
  GMidBossGenericProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute);

  ~GMidBossGenericProcess() OVERRIDE;

protected:
  TBool MaybeAttack() OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;

  void Walk(DIRECTION aDirection) OVERRIDE;

  void Move(DIRECTION aDirection) OVERRIDE;

  void Ball(DIRECTION aDirection) OVERRIDE;

  void Return(DIRECTION aDirection) OVERRIDE;

  void Revert(DIRECTION aDirection) OVERRIDE;

  void Attack(DIRECTION aDirection) OVERRIDE;

  void Hit(DIRECTION aDirection) OVERRIDE;

  void Death(DIRECTION aDirection) OVERRIDE;

  void Spell(DIRECTION aDirection) OVERRIDE;
};

#endif
