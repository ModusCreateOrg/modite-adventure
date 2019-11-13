#ifndef GLOADGAMESTATE_H
#define GLOADGAMESTATE_H

#include <BGameEngine.h>

class GLoadGameState : public BGameEngine {
public:
  GLoadGameState();
  ~GLoadGameState();

public:
  void PreRender();
  void PostRender();
};

#endif
