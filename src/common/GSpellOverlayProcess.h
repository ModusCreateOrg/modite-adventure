#ifndef MODITE_GSPELLOVERLAYPROCESS_H
#define MODITE_GSPELLOVERLAYPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"

class GSpellOverlayProcess : public GProcess {
public:
  GSpellOverlayProcess(GGameState *aGameState, GProcess *aProcess, TFloat aX, TFloat aY);

  ~GSpellOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  GAnchorSprite *mSprite;
  GProcess *mParent;
};

#endif //MODITE_GSPELLOVERLAYPROCESS_H
