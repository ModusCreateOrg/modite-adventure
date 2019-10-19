#ifndef MODITE_GDOORPROCESS_H
#define MODITE_GDOORPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GDoorProcess : public BProcess {
public:
  GDoorProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal);
  ~GDoorProcess();
public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  TUint16 mParam;
  TInt8 mGroup;
  TBool mHorizontal;
  GAnchorSprite *mSprite1;
  GAnchorSprite *mSprite2;
};

#endif //MODITE_GDOORPROCESS_H
