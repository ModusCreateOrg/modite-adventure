#ifndef GFINALBOSSPILLARSPRITE_H
#define GFINALBOSSPILLARSPRITE_H

#include "GAnchorSprite.h"
#include "GGameState.h"

class GFinalBossPillarSprite : public GAnchorSprite {
public:
  GFinalBossPillarSprite(GGameState *aGameState, TFloat aX, TFloat aY, TInt16 aSlot);

public:
  void Explode();

protected:
  TBool mExploding;
};

#endif
