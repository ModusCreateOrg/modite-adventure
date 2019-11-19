#ifndef MODITE_GLEVERPROCESS_H
#define MODITE_GLEVERPROCESS_H

#include "GEnvironmentProcess.h"

class GLeverProcess : public GEnvironmentProcess {
public:
  GLeverProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY);
  ~GLeverProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt16 mState;
  TBool mAnimating,
    mDirection; // which direction next activation moves the switch (e.g. left/right)
};

#endif //MODITE_GLEVERPROCESS_H
