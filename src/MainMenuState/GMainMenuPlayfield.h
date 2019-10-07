#ifndef GENUS_GMAINMENUPLAYFIELD_H
#define GENUS_GMAINMENUPLAYFIELD_H

#include "Game.h"

class GMainMenuPlayfield : public BPlayfield {
public:
  GMainMenuPlayfield();
  virtual ~GMainMenuPlayfield();

public:
  void Render();

public:
  BBitmap *mBackground;
  // BBitmap *mLogo;
};

#endif //GENUS_GMAINMENUPLAYFIELD_H
