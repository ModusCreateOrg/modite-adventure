#ifndef MODITE_GCRATEPROCESS_H
#define MODITE_GCRATEPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GCrateProcess : public BProcess {
public:
  GCrateProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY);
  ~GCrateProcess();
public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  TUint16 mParam;
  GAnchorSprite *mSprite;
};


#endif //MODITE_GCRATEPROCESS_H
