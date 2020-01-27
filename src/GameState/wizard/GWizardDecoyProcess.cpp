#include "GWizardDecoyProcess.h"
#include "GWizardProcess.h"

GWizardDecoyProcess::GWizardDecoyProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY,
        TUint16 aSpriteSheet) : GProcess(0, 0) {
  mParent = aParent;
  mSprite = new GAnchorSprite(aGameState, 0, BOSS_SLOT);
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->x = aX;
  mSprite->y = aY;
  aGameState->AddSprite(mSprite);
}

GWizardDecoyProcess::~GWizardDecoyProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GWizardDecoyProcess::RunBefore() {
  return ETrue;
}

TBool GWizardDecoyProcess::RunAfter() {
  if (mParent->IllusionDone()) {
    return EFalse;
  }
  return ETrue;
}
