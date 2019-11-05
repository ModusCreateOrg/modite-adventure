#ifndef MODITE_GDOORPROCESS_H
#define MODITE_GDOORPROCESS_H

#include "GEnvironmentProcess.h"

class GDoorProcess : public GEnvironmentProcess {
public:
  GDoorProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal);

  ~GDoorProcess() FINAL;

public:
  TBool RunBefore() FINAL;

  TBool RunAfter() FINAL;

protected:
  void ClearWall();

protected:
  TBool mHorizontal;
};

#endif //MODITE_GDOORPROCESS_H
