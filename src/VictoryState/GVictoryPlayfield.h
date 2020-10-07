#ifndef MODITE_GVICTORYPLAYFIELD_H
#define MODITE_GVICTORYPLAYFIELD_H

#include "Game.h"
#include "GPlayerSprite.h"
#include "GStarFieldProcess.h"

class GVictoryPlayfield : public BPlayfield {
public:
  explicit GVictoryPlayfield(GGameState *aGameState);
  ~GVictoryPlayfield() OVERRIDE;

//  TInt CenterText8(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);
  TInt CenterText16(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);

  void Animate() OVERRIDE;
  void Render() OVERRIDE;
  void DrawScrolledBackground(BBitmap *aBitmap, TFloat aOffsetX, TUint aVerticalOffset);
public:

//  BFont *mFont8;
  BFont *mFont16;
  TInt  mState;

protected:

  void CacheColors(BBitmap *aBitmap, TRGB *aColorCache) {
    TInt  numColors = aBitmap->CountUsedColors();
    TFloat pct = .25;

    for (TInt i = 0; i < numColors; i++) {
      if (i == aBitmap->TransparentColor()) {
        continue;
      }

      TRGB color = aBitmap->GetColor(i);

      aColorCache[i].Set(color.r, color.b, color.g);

      // Reduce color cache
      TUint16 red = (TFloat) color.r * pct;
      color.r = (TUint8) ((red > 0xFF) ? 0xFF : red);

      TUint16 green = (TFloat) color.g * pct;
      color.g = (TUint8) ((green > 0xFF) ? 0xFF : green);

      TUint16 blue = (TFloat) color.b * pct;
      color.b = (TUint8) ((blue > 0xFF) ? 0xFF : blue);

      aBitmap->SetColor(i, color);
    }
  }

  TRGB *mBgLogoColors;
  TRGB *mBgSkyColors;
  TRGB *mBgNearTreesColors;
  TRGB *mBgWalkingPathColors;
  TRGB *mBgMountainsColors;
  TRGB *mBgMoonColors;
  TRGB mStarsColor;

  GGameState *mGameState;

  BBitmap *mBgSky;
  BBitmap *mBgMountains;
  BBitmap *mBgMoon;
  BBitmap *mBgNearTrees;
  BBitmap *mBgWalkingPath;
  BBitmap *mBgLogo;
  GPlayerSprite *mPlayer;

  GStarFieldProcess *mStarfieldProcess;

  TFloat mSkyOffset;
  TFloat mMountainsOffset;
  TFloat mMoonOffset;
  TFloat mNearTreesOffset;
  TFloat mPathOffset;
  TRGB mSavedPalette[256];
  TInt8 mFadeState;

};

#endif //MODITE_GVICTORYPLAYFIELD_H
