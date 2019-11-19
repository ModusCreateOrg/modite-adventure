#ifndef MODITE_GSPELLOVERLAYPROCESS_H
#define MODITE_GSPELLOVERLAYPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"

class GSpellOverlayProcess : public BProcess {
public:
  GSpellOverlayProcess(GGameState *aGameState, TFloat aX, TFloat aY);

  ~GSpellOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  GAnchorSprite *GetSprite() { return mSprite; }

public:
  void KeepAlive(TBool aBool) {
    mKeepAlive = aBool;
  }

protected:
  GAnchorSprite *mSprite;
  TBool mKeepAlive = EFalse;
};

#endif //MODITE_GSPELLOVERLAYPROCESS_H
