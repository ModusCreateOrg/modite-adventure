#ifndef GLOADGAMEPLAYFIELD_H
#define GLOADGAMEPLAYFIELD_H

#include <BPlayfield.h>

class GLoadGamePlayfield : public BPlayfield {
public:
  GLoadGamePlayfield();
  ~GLoadGamePlayfield();

public:
  void Render();
};

#endif
