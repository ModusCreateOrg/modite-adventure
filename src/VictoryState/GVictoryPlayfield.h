#ifndef MODITE_GVICTORYPLAYFIELD_H
#define MODITE_GVICTORYPLAYFIELD_H

#include "Game.h"
#include "GPlayerSprite.h"
#include "GStarFieldProcess.h"
#include "GVictoryCreditsProcess.h"

class GVictoryPlayfield : public BPlayfield {
public:
  explicit GVictoryPlayfield(GGameState *aGameState);
  ~GVictoryPlayfield() OVERRIDE;

  void Render() OVERRIDE;


protected:
    void Animate() OVERRIDE;
    void DrawScrolledBackground(BBitmap *aBitmap, TFloat aOffsetX, TUint aVerticalOffset);
    void RenderAnimatedBackground();
    void StartState();
    void FadeInState();
    void FadeInFinalState();
    void RunState();
    void HoldState();

  void FadeColors() {
    for (TInt16 i = 0; i < 255; i++) {
      TRGB color = mBgColors[i];

      TUint16 red = (TFloat) color.r * mFadePct;
      color.r = (TUint8) ((red > 0xFF) ? 0xFF : red);

      TUint16 green = (TFloat) color.g * mFadePct;
      color.g = (TUint8) ((green > 0xFF) ? 0xFF : green);

      TUint16 blue = (TFloat) color.b * mFadePct;
      color.b = (TUint8) ((blue > 0xFF) ? 0xFF : blue);

      gDisplay.SetColor(i, color);
    }
  }

  void CacheColors() {
    for (TInt16 i = 0; i < 255; i++) {
      TRGB color =  gDisplay.renderBitmap->GetPalette()[i];

      mBgColors[i].Set(color.r, color.g, color.b);

      // Reduce color cache
      TUint16 red = (TFloat) color.r * mFadePct;
      color.r = (TUint8) ((red > 0xFF) ? 0xFF : red);

      TUint16 green = (TFloat) color.g * mFadePct;
      color.g = (TUint8) ((green > 0xFF) ? 0xFF : green);

      TUint16 blue = (TFloat) color.b * mFadePct;
      color.b = (TUint8) ((blue > 0xFF) ? 0xFF : blue);

      gDisplay.SetColor(i, color);
    }

  }

  TRGB *mBgColors;

  TFloat mFadePct;
  TFloat mFadeStep;

  TUint8 mSkyColorIndex;

  GGameState *mGameState;

//  BBitmap *mBgSky;
  BBitmap *mBgLabsLogo;
  BBitmap *mBgRisingSun;

  BBitmap *mBgMountains;
//  BBitmap *mBgMoon;
  BBitmap *mBgNearTrees;
  BBitmap *mBgWalkingPath;
  GPlayerSprite *mPlayer;

  GStarFieldProcess *mStarfieldProcess;
  GVictoryCreditsProcess *mCreditsProcess;

  TFloat mSkyOffset;
  TFloat mMountainsOffset;
  TFloat mSunOffset;
  TFloat mNearTreesOffset;
  TFloat mPathOffset;
  TInt32 mStateTimer;

  TInt mTimer;

  enum {
    STATE_START,
    STATE_FADEIN_INITIAL,
    STATE_RUN,
    STATE_FADEIN_FINAL,
    STATE_HOLD,
  } mState;
};

#endif //MODITE_GVICTORYPLAYFIELD_H