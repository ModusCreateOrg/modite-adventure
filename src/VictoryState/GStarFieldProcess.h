#ifndef MODITE_GSTARFIELDPROCESS_H
#define MODITE_GSTARFIELDPROCESS_H

#include "BProcess.h"
#include "Game.h"

const TInt8 NUM_STARS = 45;

class GStarFieldProcess {
public:
  explicit GStarFieldProcess() {
    mStopSpawn = EFalse;
    for (TInt i = 0; i < NUM_STARS; i++) {
      InitStar(i, 0);
    }
  }

  ~GStarFieldProcess() = default;

  void Render() {
//    Animate();
//    gDisplay.SetColor(COLOR_EXPERIENCE, mStarsColor);

    for (TInt i = 0; i < NUM_STARS; i++) {
      gDisplay.renderBitmap->DrawFastHLine(ENull, mStarX[i], mStarY[i], mStarWidth[i], COLOR_EXPERIENCE);
    }
  };

  void StopSpawn() {
    mStopSpawn = ETrue;
  }
  void Animate() {
    for (TInt i = 0; i < NUM_STARS; i++) {
      if (mStarX[i] < -10) {
        InitStar(i);
      }
      else {
        mStarX[i] -= mStarSpeed[i];
      }
    }
  }


protected:

  void InitStar(TInt aIndex, TInt32 aXStartIndex = 319) {
    if (mStopSpawn) {
      return;
    }
    mStarX[aIndex] = Random(aXStartIndex, 480);
    mStarY[aIndex] = Random(0, 110);
    mStarWidth[aIndex] = Random(1, 3);

    if (mStarWidth[aIndex] >= 3) {
      mStarSpeed[aIndex] = Random(20, 30) * 0.01f;
    }
    else if (mStarWidth[aIndex] >= 2) {
      mStarSpeed[aIndex] = Random(15, 20) * 0.01f;
    }
    else {
      mStarSpeed[aIndex] = Random(3, 10) * 0.01f;
    }

  }

  TBool mStopSpawn;
  TFloat mStarX[NUM_STARS]{};
  TFloat mStarY[NUM_STARS]{};
  TFloat mStarWidth[NUM_STARS]{};
  TFloat mStarSpeed[NUM_STARS]{};
};


#endif //MODITE_GSTARFIELDPROCESS_H
