#ifndef MODITE_GPOTPROCESS_H
#define MODITE_GPOTPROCESS_H


#include "GEnvironmentProcess.h"

class GPotProcess : public GEnvironmentProcess {
public:
  GPotProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY);

  ~GPotProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  TBool mAnimating;
};


#endif //MODITE_GPOTPROCESS_H
