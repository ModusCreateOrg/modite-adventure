#ifndef MODITE_GPLAYERSPRITE_H
#define MODITE_GPLAYERSPRITE_H

#include "GAnchorSprite.h"

class GPlayerSprite : public GAnchorSprite {
public:
  EXPLICIT GPlayerSprite(GGameState *aGameState);

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;
};

#endif //MODITE_GPLAYERSPRITE_H
