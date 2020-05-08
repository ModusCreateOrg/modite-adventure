#ifndef MODITE_GITEMPROCESS_H
#define MODITE_GITEMPROCESS_H

#include "GGameState.h"
#include "GAnchorSprite.h"
#include "GProcess.h"

class GItemProcess : public GProcess {
public:
  GItemProcess(GGameState *aGameState, TInt aIp, TInt mItemNumber, TFloat aX, TFloat aY);

public:
  ~GItemProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  static GItemProcess *SpawnItem(GGameState *aGameState, TInt aIp, TInt mItemNumber, TFloat aX, TFloat aY);

protected:
  GGameState *mGameState;
  TInt mItemNumber,
      mIp;
  GAnchorSprite *mSprite;
  TBool mFromPot;
};

#endif //MODITE_GITEMPROCESS_H
