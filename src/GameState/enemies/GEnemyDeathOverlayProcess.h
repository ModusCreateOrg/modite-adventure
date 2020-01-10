#ifndef MODITE_GENEMYDEATHOVERLAYPROCESS_H
#define MODITE_GENEMYDEATHOVERLAYPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"
#include "GResources.h"

class GEnemyDeathOverlayProcess : public GProcess {
public:
  GEnemyDeathOverlayProcess(GGameState *aGameState, GProcess *aProcess, TFloat aX, TFloat aY);

  ~GEnemyDeathOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  GAnchorSprite *mSprite;
  GProcess *mParent;
};


#endif //MODITE_GENEMYDEATHOVERLAYPROCESS_H
