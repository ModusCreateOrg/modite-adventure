#ifndef MODITE_GENEMYPROCESS_H
#define MODITE_GENEMYPROCESS_H

#include <BProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

enum {
  IDLE_STATE,
  WALK_STATE,
  ATTACK_STATE,
  HIT_STATE
};

class GEnemyProcess : public BProcess {
public:
  GEnemyProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TUint16 aSlot, TUint16 aPalette, TUint16 aColors);

  virtual ~GEnemyProcess();

protected:
  GGameState *mGameState;
  GGamePlayfield *mGamePlayfield;
  GAnchorSprite *mSprite;
  TUint16 mState;
  TUint16 mDirection;
  TUint16 mStep;

protected:
  virtual void NewState(TUint16 aState, TUint16 aDirection) = 0;

  virtual TBool IdleState() = 0;

  virtual TBool WalkState() = 0;

  virtual TBool AttackState() = 0;

  virtual TBool HitState() = 0;
};


#endif //MODITE_GENEMYPROCESS_H
