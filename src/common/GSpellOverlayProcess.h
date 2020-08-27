#ifndef MODITE_GSPELLOVERLAYPROCESS_H
#define MODITE_GSPELLOVERLAYPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"

class GSpellOverlayProcess : public GProcess {
public:
  GSpellOverlayProcess(GGameState *aGameState, GProcess *aProcess, TFloat aX, TFloat aY, TInt16 aSpawnDelay,
                       TFloat aVX, TFloat aVY);
  TInt16 mTimer;
  ~GSpellOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
    TInt16 mSpawnDelay;
  GAnchorSprite *mSprite;
  GProcess *mParent;
  GGameState *mGameState;
};

#endif //MODITE_GSPELLOVERLAYPROCESS_H
