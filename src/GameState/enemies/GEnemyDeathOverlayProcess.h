#ifndef MODITE_GENEMYDEATHOVERLAYPROCESS_H
#define MODITE_GENEMYDEATHOVERLAYPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GResources.h"

class GEnemyDeathOverlayProcess : public BProcess {
public:
  GEnemyDeathOverlayProcess(GGameState *aGameState, TFloat aX, TFloat aY);

  ~GEnemyDeathOverlayProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  GAnchorSprite *GetSprite() { return mSprite; }

protected:
  GAnchorSprite *mSprite;
};


#endif //MODITE_GENEMYDEATHOVERLAYPROCESS_H
