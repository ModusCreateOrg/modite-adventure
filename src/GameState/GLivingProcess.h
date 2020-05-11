#ifndef MODITE_GLIVINGPROCESS_H
#define MODITE_GLIVINGPROCESS_H

#include "GProcess.h"
#include "GResources.h"
#include "GPlayer.h"

class GLivingProcess : public GProcess {
public:
  EXPLICIT GLivingProcess(GGameState *aGameState, TUint16 aAttribute);
  ~GLivingProcess() OVERRIDE;

  void SetStatMultipliers(TFloat aModHitPoints = 1.0, TFloat aModStrength = 1.0, TFloat aModExperience = 1.0);

  TBool MaybeDamage(TBool aIsSpell);

protected:
  void StartBlink(TUint16 aTime) {
    mBlinkTimer = aTime;
  }

private:
  virtual void BlinkOn() {
    if (mSprite) {
      mSprite->mFill = COLOR_WHITE;
    }
  };

  virtual void BlinkOff() {
    if (mSprite) {
      mSprite->mFill = -1;
    }
  };

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  TBool mInvulnerable;   // cannot be attacked
  TInt16 mHitPoints, mMaxHitPoints;
  TInt32 mExperienceYield;

protected:
  GGameState *mGameState;
  TUint16 mState, mStep; // state variables

private:
  TUint16 mBlinkTimer;
};

#endif //MODITE_GLIVINGPROCESS_H
