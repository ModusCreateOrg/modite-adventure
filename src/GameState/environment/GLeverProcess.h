#ifndef MODITE_GLEVERPROCESS_H
#define MODITE_GLEVERPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GLeverProcess : public BProcess {
public:
  GLeverProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY);
  ~GLeverProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  TUint16 mParam;
  TInt16 mState;
  TBool mAnimating, mDirection;
  GAnchorSprite *mSprite;
};

#endif //MODITE_GLEVERPROCESS_H
