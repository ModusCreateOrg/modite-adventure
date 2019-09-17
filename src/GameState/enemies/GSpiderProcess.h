#ifndef MODITE_GSPIDERPROCESS_H
#define MODITE_GSPIDERPROCESS_H

#include "GameState/GEnemyProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSpiderProcess : public GEnemyProcess {
public:
  GSpiderProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);
  
  ~GSpiderProcess();

public:
  TBool RunBefore() override;
  
  TBool RunAfter() override;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;
  
  TBool MaybeHit();
  
  TBool MaybeAttack();
  
  TBool CanWalk();
  
  TBool IdleState() override;
  
  TBool WalkState() override;
  
  TBool AttackState() override;
  
  TBool HitState() override;
  
  TBool DeathState() override;

protected:
  TFloat mStartX, mStartY;
  TInt16 mStateTimer;
};

#endif //MODITE_GSPIDERPROCESS_H
