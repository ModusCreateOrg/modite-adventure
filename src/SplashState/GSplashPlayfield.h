#ifndef MODITE_GSPLASHPLAYFIELD_H
#define MODITE_GSPLASHPLAYFIELD_H

#include "Game.h"

typedef struct {
  TInt index;
  float x, y, z;
  float screen_x, screen_y;
  TUint16 color;
} Particle;


class GSplashPlayfield : public BPlayfield {
public:
  GSplashPlayfield();

  virtual ~GSplashPlayfield();

  void Render() final;
  void Animate() final;

  void InitParticles();

public:
  BBitmap *mBackground;
  TRGB mSavedPalette[256];
  TInt mNumParticles;
  Particle *mParticles;
  TInt16 mNumParticlesMoved;
  TUint16 mFrame;
  TFloat mAnimatedColorValue;
};

#endif //MODITE_GSPLASHPLAYFIELD_H
