#ifndef MODITE_GSPIKESPROCESS_H
#define MODITE_GSPIKESPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GSpikesProcess : public BProcess {
public:
  GSpikesProcess(GGameState *aGameState, TFloat aX, TFloat aY);

  ~GSpikesProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

public:
  static TInt mNumber;
protected:
  GGameState    *mGameState;
  TUint16       mParam;
  GAnchorSprite *mSprite;
  TBool         mState;   // true if animating
  TInt          mTimer, mTime;
};


#endif //MODITE_GSPIKESPROCESS_H
