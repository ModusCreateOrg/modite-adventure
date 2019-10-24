#include "GMidBossFireProcess.h"

const TInt16 IDLE_SPEED = 3;

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 2),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 3),
  ALOOP,
};

GMidBossFireProcess::GMidBossFireProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot) 
: GMidBossProcess(aGameState, aX, aY, aSlot) {
  //
}

GMidBossFireProcess::~GMidBossFireProcess() {
  //
}

void GMidBossFireProcess::Idle(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Walk(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Ball(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Return(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Revert(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

