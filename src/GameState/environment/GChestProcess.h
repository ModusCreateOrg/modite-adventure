#ifndef MODITE_GCHESTPROCESS_H
#define MODITE_GCHESTPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GChestProcess : public BProcess {
public:
  GChestProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY);
  ~GChestProcess();
public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  TUint16 mParam;
  GAnchorSprite *mSprite;
};


#endif //MODITE_GCHESTPROCESS_H
