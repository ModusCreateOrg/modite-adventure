#include "GWizardDeathProcess.h"
#include "GGameState.h"

const TInt16 DEATH_SPEED = 4;

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_DEATH_SLOT),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 0),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 1),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 2),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 3),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 4),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 5),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 6),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 7),
  AEND,
};

GWizardDeathProcess::GWizardDeathProcess(GGameState *aGameState, GWizardProcess *aWizard, TFloat aX, TFloat aY, TInt aDelay)
    : GProcess(ATTR_GONE) {
  mGameState = aGameState;
  mWizard = aWizard;
  mX = aX - 16 + Random(0, 18 / 2);
  mY = aY + Random(0, 75 / 2);
  mTimer = aDelay * DEATH_SPEED * 3;
  mSprite = ENull;
}

GWizardDeathProcess::~GWizardDeathProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  mWizard->DeathAnimationDone();
}

TBool GWizardDeathProcess::RunBefore() {
  if (--mTimer <= 0 && !mSprite) {
    mSprite = new BAnimSprite(ENEMY_DEATH_PRIORITY, ENEMY_DEATH_SLOT, 0, STYPE_DEFAULT);
    mSprite->x = mX;
    mSprite->y = mY;
    mSprite->SetFlags(SFLAG_BELOW);
    mGameState->AddSprite(mSprite);
    mSprite->StartAnimation(deathAnimation);
    gSoundPlayer.TriggerSfx(SFX_WIZARD_EXPLODING_WAV, 4);
  }
  return ETrue;
}

TBool GWizardDeathProcess::RunAfter() {
  if (mSprite && mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}
