#ifndef MODITE_GELEMENTALSKULLPROCESS_H
#define MODITE_GELEMENTALSKULLPROCESS_H

#include <GameState/enemies/GGruntProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GElementalSkullProcess : public GGruntProcess {
public:
  GElementalSkullProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam, TUint16 aAttribute);
  ~GElementalSkullProcess() OVERRIDE;
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

protected:
  TBool HitState() OVERRIDE;
  TBool SpellState() OVERRIDE {
    return HitState();
  };
  TBool MaybeAttack() OVERRIDE { return EFalse; };
  TBool BasicDamageCheck() OVERRIDE;
  TBool SpellDamageCheck() OVERRIDE;
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE {};
  void Walk(DIRECTION aDirection) OVERRIDE {};
  void Attack(DIRECTION aDirection) OVERRIDE {};
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Spell(DIRECTION aDirection) OVERRIDE { Hit(aDirection); };
  void Death(DIRECTION aDirection) OVERRIDE;
  void ResetTimer();

public:
  static TUint8 mInstances;

protected:
  TUint8  mTimer;
  TFloat mSpriteX, mSpriteY;
  ANIMSCRIPT *mAnimation;
};


#endif //MODITE_GELEMENTALSKULLPROCESS_H

