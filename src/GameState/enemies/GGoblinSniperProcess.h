#ifndef MODITE_GGOBLINSNIPERPROCESS_H
#define MODITE_GGOBLINSNIPERPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GGoblinSniperProcess : public GEnemyProcess {
public:
  GGoblinSniperProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GGoblinSniperProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

  TBool DeathState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GGOBLINSNIPERPROCESS_H
