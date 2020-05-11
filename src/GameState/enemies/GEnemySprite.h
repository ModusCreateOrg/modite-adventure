#ifndef GENEMYSPRITE_H
#define GENEMYSPRITE_H

#include "GAnchorSprite.h"

class GEnemySprite : public GAnchorSprite {
public:
  GEnemySprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg = 0, TUint16 aType = STYPE_DEFAULT);

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;

public:
  TFloat mMeter;
};

#endif
