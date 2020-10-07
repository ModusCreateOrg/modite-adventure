#ifndef MODITE_GSTARFIELDPROCESS_H
#define MODITE_GSTARFIELDPROCESS_H

#include "BProcess.h"
#include "Game.h"

const TInt NUM_STARS = 30;

class GStarFieldProcess {
public:
  explicit GStarFieldProcess() {
    for (TInt i = 0; i < NUM_STARS; i++) {
      InitStar(i, 0);
    }
  }

  ~GStarFieldProcess() = default;


  void Render() {
    Animate();
//  gDisplay.renderBitmap->Clear(86);
    for (TInt i = 0; i < NUM_STARS; i++) {
      gDisplay.renderBitmap->DrawFastHLine(ENull, mStarX[i], mStarY[i], mStarWidth[i], COLOR_EXPERIENCE);
    }
  };


protected:

  void InitStar(TInt aIndex, TInt32 aXStartIndex = 320) {
    mStarX[aIndex] = Random(aXStartIndex, 480);
    mStarY[aIndex] = Random(0, 80);
    mStarWidth[aIndex] = Random(1, 3);
//    mStarSpeed[aIndex] = Random(10, 40) * 0.01f;
//    printf("INIT %i -- x(%i), y(%i), speed(%2f)\n", aIndex, mStarX[aIndex], mStarY[aIndex], mStarSpeed[aIndex]);


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
  void Animate() {
    for (TInt i = 0; i < NUM_STARS; i++) {
//      printf("i=%i - x(%i), y(%i), speed(%2f)\n", i, (TInt16)mStarX[i], (TInt16)mStarY[i], (TInt16)mStarSpeed[i]);

      if (mStarX[i] < -10) {
        InitStar(i);
      }
      else {
        mStarX[i] -= mStarSpeed[i];
      }
    }
  }

    TFloat mStarX[NUM_STARS]{},
       mStarY[NUM_STARS]{},
       mStarWidth[NUM_STARS]{};

  TFloat mStarSpeed[NUM_STARS]{};

};


#endif //MODITE_GSTARFIELDPROCESS_H
