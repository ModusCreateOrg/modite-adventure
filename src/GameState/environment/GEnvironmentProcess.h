#ifndef GENVIRONMENTPROCESS_H
#define GENVIRONMENTPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "object_layer_attributes.h"
#include "inventory/GItemProcess.h"

const TInt ENVIRONMENT_PRIORITY_DEFAULT = 0;    // default priority
const TInt ENVIRONMENT_PRIORITY_DOOR = -10;     // doors run after switches for group done logic to work

class GEnvironmentProcess : public BProcess {
public:
  GEnvironmentProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TInt aPri = ENVIRONMENT_PRIORITY_DEFAULT);
  ~GEnvironmentProcess();

protected:
  GGameState *mGameState;
  TUint16 mParam;
  OBJECT_ATTRIBUTE *mAttribute;
//  TInt mGroup, mOrder;
  GAnchorSprite *mSprite, *mSprite1, *mSprite2;
};

#endif
