#ifndef MODITE_GFLOOR_SWITCHPROCESS_H
#define MODITE_GFLOOR_SWITCHPROCESS_H

#include "GEnvironmentProcess.h"

class GFloorSwitchProcess : public GEnvironmentProcess {
public:
  GFloorSwitchProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden);
  ~GFloorSwitchProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TUint16 mImage;
  TBool mState, mAnimating;
};

#endif //MODITE_GFLOOR_SWITCHPROCESS_H
