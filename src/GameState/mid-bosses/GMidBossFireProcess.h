#ifndef GMIDBOSSFIREPROCESS_H
#define GMIDBOSSFIREPROCESS_H

#include "GMidBossProcess.h"

class GMidBossFireProcess : public GMidBossProcess {
public:
  GMidBossFireProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot);
  ~GMidBossFireProcess();
public:
  void Idle(DIRECTION aDirection);
  void Walk(DIRECTION aDirection);
  void Ball(DIRECTION aDirection);
  void Return(DIRECTION aDirection);
  void Revert(DIRECTION aDirection);
  void Attack(DIRECTION aDirection);
  void Hit(DIRECTION aDirection);
  void Death(DIRECTION aDirection);
  void Spell(DIRECTION aDirection);
};

#endif

