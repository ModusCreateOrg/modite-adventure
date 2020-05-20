#ifndef MODITE_GTURRETPROCESS_H
#define MODITE_GTURRETPROCESS_H

#include <GameState/enemies/GGruntProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GTurretProcess : public GGruntProcess {
public:
  GTurretProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam);
  ~GTurretProcess() OVERRIDE;
  TBool RunBefore() OVERRIDE;
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

protected:
  TBool HitState() OVERRIDE;
  TBool SpellState() OVERRIDE {
    return HitState();
  };
  TBool MaybeAttack() OVERRIDE;
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE {};
  void Walk(DIRECTION aDirection) OVERRIDE {};
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Spell(DIRECTION aDirection) OVERRIDE {
    Hit(aDirection);
  };
  void Death(DIRECTION aDirection) OVERRIDE;

protected:
  TInt mHitState;
  TInt8 mShimmerTimer;
  TUint8 mSpriteImageNumber;
};


#endif //MODITE_GTURRETPROCESS_H

