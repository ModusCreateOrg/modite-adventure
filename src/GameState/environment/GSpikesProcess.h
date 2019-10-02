#ifndef MODITE_GSPIKESPROCESS_H
#define MODITE_GSPIKESPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GSpikesProcess : public BProcess {
public:
  GSpikesProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParam);

  ~GSpikesProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  static TInt mNumber;

protected:
  GGameState *mGameState;
  TUint16 mParam;
  GAnchorSprite *mSprite;
  TBool mState;   // true if animating
  TInt mTimer, mTime;
};


#endif //MODITE_GSPIKESPROCESS_H
