#ifndef MODITE_GTITLEPLAYFIELD_H
#define MODITE_GTITLEPLAYFIELD_H

#include "Game.h"

class GTitlePlayfield : public BPlayfield {
public:
  GTitlePlayfield();
  virtual ~GTitlePlayfield();
  void Render();
public:
  BBitmap *mBackground;
};


#endif //MODITE_GTITLEPLAYFIELD_H
