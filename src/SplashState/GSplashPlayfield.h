#ifndef MODITE_GSPLASHPLAYFIELD_H
#define MODITE_GSPLASHPLAYFIELD_H

#include "Game.h"

class GSplashPlayfield : public BPlayfield {
public:
  GSplashPlayfield();

  virtual ~GSplashPlayfield();

  void Render() final;
  void Animate() final;

public:
  BBitmap *mBackground;
  TRGB mSavedPalette[27];
  TInt16 mYPosition;
  TFloat mYStep;
  TUint16 mFrame;
  TUint16 mBitmapHeight;
  TUint16 mBackgroundX;
  TUint16 mBackgroundY;
  TFloat mAnimatedColorValue;
};

#endif //MODITE_GSPLASHPLAYFIELD_H
