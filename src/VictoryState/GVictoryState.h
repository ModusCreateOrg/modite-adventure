#ifndef GVICTORYSTATE_H
#define GVICTORYSTATE_H

#include <BGameEngine.h>

class GVictoryState : public BGameEngine {
public:
  GVictoryState(GGameState *aGameState);
  ~GVictoryState() OVERRIDE;
};

#endif
