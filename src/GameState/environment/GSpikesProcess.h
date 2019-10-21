#ifndef MODITE_GSPIKESPROCESS_H
#define MODITE_GSPIKESPROCESS_H

#include "GEnvironmentProcess.h"

class GSpikesProcess : public GEnvironmentProcess {
public:
  GSpikesProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParam);

  ~GSpikesProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  static TInt mNumber;

protected:
  TBool mState;   // true if animating
  TInt mTimer, mTime;
};


#endif //MODITE_GSPIKESPROCESS_H
