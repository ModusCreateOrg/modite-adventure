#ifndef MODITE_GCHESTPROCESS_H
#define MODITE_GCHESTPROCESS_H

#include "GEnvironmentProcess.h"

class GChestProcess : public GEnvironmentProcess {
public:
  GChestProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY);
  ~GChestProcess();

public:
  TBool RunBefore();
  TBool RunAfter();
};

#endif //MODITE_GCHESTPROCESS_H
