#ifndef MODITE_GFINALBOSSSHIELDPROCESS_H
#define MODITE_GFINALBOSSSHIELDPROCESS_H

#include "GProcess.h"
#include "GFinalBossPillarSprite.h"

const TInt SPRITE_COUNT = 8;
const TFloat ANGULAR_VELOCITY = 2.0 / FRAMES_PER_SECOND; // radians/frame

class GFinalBossShieldProcess : public GProcess {
public:
  EXPLICIT GFinalBossShieldProcess(GGameState *aGameState, TInt16 aSlot);
  ~GFinalBossShieldProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE { return ETrue; };
  TBool RunAfter() OVERRIDE;

public:
  void UpdateCenter(const TPoint &aPoint);

  void KillShield();

protected:
  TPoint mCenter{};
  GFinalBossPillarSprite *mSprites[SPRITE_COUNT]{};
  TFloat mAngle;
};


#endif //MODITE_GFINALBOSSSHIELDPROCESS_H
