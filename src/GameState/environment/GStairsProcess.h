#ifndef MODITE_GSTAIRSPROCESS_H
#define MODITE_GSTAIRSPROCESS_H

#include "GEnvironmentProcess.h"

class GStairsProcess : public GEnvironmentProcess {
public:
  GStairsProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aLevel, TFloat aX, TFloat aY, const char *aKind);

  ~GStairsProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  GAnchorSprite *mSprite1;
  GAnchorSprite *mSprite2;
  DIRECTION mDirection; // UP or DOWN
  TInt mLevel;          // target level (where stairs lead)
};

#endif //MODITE_GSTAIRSPROCESS_H
