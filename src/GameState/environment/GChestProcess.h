#ifndef MODITE_GCHESTPROCESS_H
#define MODITE_GCHESTPROCESS_H

#include "GEnvironmentProcess.h"

class GChestProcess : public GEnvironmentProcess {
public:
  GChestProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aIsOpen);
  ~GChestProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
};

#endif //MODITE_GCHESTPROCESS_H
