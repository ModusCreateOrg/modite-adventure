#ifndef MODITE_GDOORPROCESS_H
#define MODITE_GDOORPROCESS_H

#include "GEnvironmentProcess.h"

class GDoorProcess : public GEnvironmentProcess {
public:
  GDoorProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal);
  ~GDoorProcess();
public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  TBool mHorizontal;
};

#endif //MODITE_GDOORPROCESS_H
