#include "GFinalBossShieldProcess.h"

GFinalBossShieldProcess::GFinalBossShieldProcess(GGameState *aGameState, TInt16 aSlot) : GProcess(0) {
  for (auto &s : mSprites) {
    s = new GFinalBossPillarSprite(aGameState, 0, 0, aSlot);
    s->SetFlags(SFLAG_CHECK);
    s->SetCMask(STYPE_PLAYER);
    aGameState->AddSprite(s);
  }
  mAngle = 0.0;
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
  for (auto &s : mSprites) {
    s->Explode();
  }
}

TBool GFinalBossShieldProcess::RunAfter() {
  mAngle += ANGULAR_VELOCITY;
  if (mAngle >= 2 * M_PI) {
    mAngle -= 2 * M_PI;
  }

  for (TInt i = 0; i < SPRITE_COUNT; i++) {
    mSprites[i]->x = mCenter.x + 48 * SIN(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) - 16;
    mSprites[i]->y = mCenter.y + 24 * COS(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) + 4;
    mSprites[i]->ClearCType(STYPE_PLAYER);
  }

  return !mSprites[0]->AnimDone();
}
