#ifndef MODITE_GSPELLOVERLAYPROCESS_H
#define MODITE_GSPELLOVERLAYPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"

class GSpellOverlayProcess : public GProcess {
public:
  GSpellOverlayProcess(GGameState *aGameState, TFloat aX, TFloat aY);

  ~GSpellOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  GAnchorSprite *GetSprite() { return mSprite; }

protected:
  GAnchorSprite *mSprite;
};

#endif //MODITE_GSPELLOVERLAYPROCESS_H
