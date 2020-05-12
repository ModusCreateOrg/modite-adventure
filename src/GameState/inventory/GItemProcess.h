#ifndef MODITE_GITEMPROCESS_H
#define MODITE_GITEMPROCESS_H

#include "GGameState.h"
#include "GAnchorSprite.h"
#include "GProcess.h"

class GItemProcess : public GProcess {
public:
  GItemProcess(GGameState *aGameState, TInt aIp, TInt mItemNumber, TFloat aX, TFloat aY);

public:
  ~GItemProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

public:
  static GItemProcess *SpawnItem(GGameState *aGameState, TInt aIp, TInt mItemNumber, TFloat aX, TFloat aY);

protected:
  GGameState *mGameState;
  TInt mItemNumber,
      mIp;
  GAnchorSprite *mSprite;
  // Following line from https://github.com/ModusCreateOrg/modite-adventure/pull/503/files
//  TBool mFromPot;
};

#endif //MODITE_GITEMPROCESS_H
