#ifndef MODITE_GSTAIRSPROCESS_H
#define MODITE_GSTAIRSPROCESS_H

#include <BProcess.h>
#include <GAnchorSprite.h>
#include <GGamePlayfield.h>
#include <GGameState.h>

class GStairsProcess : public BProcess {
public:
  GStairsProcess(GGameState *aGameState, DIRECTION aDirection, TInt aLevel, TFloat aX, TFloat aY);

  ~GStairsProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  GGameState    *mGameState;
  GAnchorSprite *mSprite1;
  GAnchorSprite *mSprite2;
  DIRECTION     mDirection;      // UP or DOWN
  TInt          mLevel;                // target level (where stairs lead)
};


#endif //MODITE_GSTAIRSPROCESS_H
