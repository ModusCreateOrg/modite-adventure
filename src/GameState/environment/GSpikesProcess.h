#ifndef MODITE_GSPIKESPROCESS_H
#define MODITE_GSPIKESPROCESS_H

#include "GEnvironmentProcess.h"

const TUint8 MAX_SPIKE_GROUPS = 255;

class GSpikesProcess : public GEnvironmentProcess {
public:
  GSpikesProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam);

  ~GSpikesProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  static TInt mGroups[MAX_SPIKE_GROUPS];

protected:
  TBool mState;   // true if animating
  TInt mTimer, mTime;
};


#endif //MODITE_GSPIKESPROCESS_H
