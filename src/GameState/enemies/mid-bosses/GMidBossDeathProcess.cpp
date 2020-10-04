#include "GMidBossDeathProcess.h"
#include "GGameState.h"

const TInt16 DEATH_SPEED = 5;

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_DEATH_SLOT),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 0),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 1),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 2),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 3),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 4),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 5),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 6),
  ASTEP(DEATH_SPEED, IMG_MID_BOSS_DEATH + 7),
  AEND,
};

GMidBossDeathProcess::GMidBossDeathProcess(GGameState *aGameState, GMidBossProcess *aMidBoss, TFloat aX, TFloat aY, TInt aDelay)
    : GProcess(ATTR_GONE) {
  mGameState = aGameState;
  mMidBoss = aMidBoss;
  mX = aX - 16 + Random(0, 44 / 2);
  mY = aY + Random(0, 75 / 2);
  mTimer = aDelay * DEATH_SPEED * 3;
  mSprite = ENull;
}

GMidBossDeathProcess::~GMidBossDeathProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  mMidBoss->DeathAnimationDone();
}

TBool GMidBossDeathProcess::RunBefore() {
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

TBool GMidBossDeathProcess::RunAfter() {
  if (mSprite && mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}
