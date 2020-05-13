#ifndef GENUS_GMAINMENUPLAYFIELD_H
#define GENUS_GMAINMENUPLAYFIELD_H

#include "Game.h"
#include "GPlayerSprite.h"
class GMainMenuPlayfield : public BPlayfield {
public:
  GMainMenuPlayfield(GGameState *aGameState);
  virtual ~GMainMenuPlayfield();

  TInt CenterText8(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);
  TInt CenterText16(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);
    void Animate() override;
    void Render() override;
    void DrawScrolledBackground(BBitmap *aBitmap, TFloat aOffsetX, TUint aVerticalOffset, TBool aDrawTransparent);
public:

  BFont   *mFont8, *mFont16;
  TInt    mState;

protected:

  GGameState *mGameState;

  BBitmap *mBgSky;
  BBitmap *mBgMountains;
  BBitmap *mBgMoon;
  BBitmap *mBgNearTrees;
  BBitmap *mBgWalkingPath;
  BBitmap *mBgLogo;
  GPlayerSprite *mPlayer;


  TFloat mSkyOffset;
  TFloat mMountainsOffset;
  TFloat mMoonOffset;
  TFloat mNearTreesOffset;
  TFloat mPathOffset;
};

#endif //GENUS_GMAINMENUPLAYFIELD_H
