#include "GWizardDecoyProcess.h"
#include "GWizardProcess.h"

const TInt16 ATTACK_SPEED = 2 * FACTOR;

static ANIMSCRIPT channelingAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(ATTACK_SPEED * 6, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED * 4, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 1),
  ALOOP,
};

GWizardDecoyProcess::GWizardDecoyProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY,
        TUint16 aSpriteSheet) : GProcess(0, 0) {
  mParent = aParent;
  mSprite = new GAnchorSprite(aGameState, 0, BOSS_SLOT);
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->x = aX;
  mSprite->y = aY;
  aGameState->AddSprite(mSprite);
  mSprite->StartAnimation(channelingAnimation);
  printf("Decoy spawned %f, %f\n", aX, aY);
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
  if (!mParent->IsChanneling()) {

    return EFalse;
  }
  return ETrue;
}
