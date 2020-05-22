#ifndef GENVIRONMENTPROCESS_H
#define GENVIRONMENTPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "object_layer_attributes.h"
#include "inventory/GItemProcess.h"

const TInt ENVIRONMENT_PRIORITY_DEFAULT = 0;    // default priority
const TInt ENVIRONMENT_PRIORITY_DOOR = 10;     // doors run after switches for group done logic to work

class GEnvironmentProcess : public GProcess {
public:
  GEnvironmentProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TInt aPri = ENVIRONMENT_PRIORITY_DEFAULT);
  ~GEnvironmentProcess() OVERRIDE;

protected:
  GGameState *mGameState;
  TInt mIp;   // instruction pointer of ObjectProgram in map
  TUint16 mParam;
  OBJECT_ATTRIBUTE *mObjectAttribute;
//  TInt mGroup, mOrder;
//  GAnchorSprite *mSprite,  *mSprite2;
};

#endif
