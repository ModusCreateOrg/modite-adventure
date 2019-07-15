#ifndef MODITE_GSPLASHPLAYFIELD_H
#define MODITE_GSPLASHPLAYFIELD_H

#include "Game.h"

class GSplashPlayfield : public BPlayfield {
public:
  GSplashPlayfield();

  virtual ~GSplashPlayfield();

  void Render();

public:
  BBitmap *mBackground;
};

#endif //MODITE_GSPLASHPLAYFIELD_H
