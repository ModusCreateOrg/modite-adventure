#include "GFinalBossShieldProcess.h"

GFinalBossShieldProcess::GFinalBossShieldProcess(GGameState *aGameState, ELEMENT aElement) : GProcess(0) {
  for (auto &s : mSprites) {
    s = new GFinalBossPillarSprite(aGameState, 0, 0, aElement);
    s->SetFlags(SFLAG_CHECK);
    s->SetCMask(STYPE_PLAYER);
    aGameState->AddSprite(s);
  }
  mAngle = 0.0;
  mExploding = EFalse;
}

GFinalBossShieldProcess::~GFinalBossShieldProcess() {
  for (auto &s : mSprites) {
    s->Remove();
    delete s;
    s = ENull;
  }
}

void GFinalBossShieldProcess::UpdateCenter(const TPoint &aPoint) {
  mCenter.Set(aPoint);
}

void GFinalBossShieldProcess::KillShield() {
  mExploding = ETrue;
  for (TInt i = 0; i < SPRITE_COUNT; i++) {
    auto &s = mSprites[i];
    s->vx = ANGULAR_VELOCITY * 48 * SIN(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT);
    s->vy = ANGULAR_VELOCITY * 24 * COS(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT);
  }
}

TBool GFinalBossShieldProcess::RunAfter() {
  mAngle += ANGULAR_VELOCITY;
  if (mAngle >= 2 * M_PI) {
    mAngle -= 2 * M_PI;
  }

  for (TInt i = 0; i < SPRITE_COUNT; i++) {
    auto &s = mSprites[i];
    if (mExploding) {
      if (!s->CanWalk(s->vx, s->vy, ETrue)) {
        s->vx = s->vy = 0;
        s->Explode();
      }
      if (s->AnimDone()) {
        s->ClearFlags(SFLAG_RENDER);
      }
    } else {
      s->x = mCenter.x + 48 * SIN(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) - 16;
      s->y = mCenter.y + 24 * COS(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) + 4;
    }
    s->ClearCType(STYPE_PLAYER);
  }
  for (auto &s : mSprites) {
    if (!s->AnimDone()) {
      return ETrue;
    }
  }
  return EFalse;
}
