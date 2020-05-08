#ifndef GLOADGAMESTATE_H
#define GLOADGAMESTATE_H

#include <BGameEngine.h>
#include "GResources.h"

class GSavedGameNode;

class GLoadGameState : public BGameEngine {
public:
  GLoadGameState();

  ~GLoadGameState() OVERRIDE;

  void Reload();

public:
  void PreRender() OVERRIDE;

  void PostRender() OVERRIDE;

protected:
  TInt StringLength8(const char *s) {
    return strlen(s) * 8;
  }

  TInt StringLength16(const char *s) {
    return strlen(s) * 12;
  }

  TInt CenterTextShadow16(const char *s, TInt aY, TInt aColor, TInt aBackground, TInt aShadow);

  TInt CenterText8(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);

  TInt CenterText16(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);

  TInt Text8(const char *s, TInt aX, TInt aY, TInt aColor, TInt aBackground);

  TInt Text16(const char *s, TInt aX, TInt aY, TInt aColor, TInt aBackground);

protected:
  TBool mConfirm, mBlinkState;
  TInt mBlinkTimer;
  TInt mTop, mHeight, mCurrent;
  GSavedGameNode *mCurrentNode;
};

#endif
