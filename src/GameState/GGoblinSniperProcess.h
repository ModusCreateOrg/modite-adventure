#ifndef MODITE_GGOBLINSNIPERPROCESS_H
#define MODITE_GGOBLINSNIPERPROCESS_H

#include <GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GGoblinSniperProcess : public GEnemyProcess {
public:
  GGoblinSniperProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GGoblinSniperProcess();

public:
  TBool RunBefore() override;

  TBool RunAfter() override;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

  TBool MaybeHit();

  TBool IdleState() override;

  TBool WalkState() override;

  TBool AttackState() override;

  TBool HitState() override;

  TBool DeathState() override;
protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GGOBLINSNIPERPROCESS_H
