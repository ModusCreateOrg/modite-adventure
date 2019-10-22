#ifndef MODITE_GCRATEPROCESS_H
#define MODITE_GCRATEPROCESS_H


#include "GEnvironmentProcess.h"

class GCrateProcess : public GEnvironmentProcess {
public:
  GCrateProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY);
  ~GCrateProcess();
public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  TBool mAnimating;
};


#endif //MODITE_GCRATEPROCESS_H
