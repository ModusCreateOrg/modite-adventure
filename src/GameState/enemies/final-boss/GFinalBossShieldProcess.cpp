#include "GFinalBossShieldProcess.h"

GFinalBossShieldProcess::GFinalBossShieldProcess(GGameState *aGameState, ELEMENT aElement) : GProcess(0) {
  for (auto &s : mSprites) {
    s = new GFinalBossPillarSprite(aGameState, 0, 0, aElement);
    aGameState->AddSprite(s);
    s->Persist();
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
    s->w = 16;
    s->h = 8;
    s->cx = 0;
    s->cy = 4;
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
        s->ClearFlags(SFLAG_RENDER | SFLAG_CHECK);
      }
    } else {
      s->x = mCenter.x + 55 * SIN(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) - 16;
      s->y = mCenter.y + 30 * COS(mAngle + 2 * M_PI * TFloat(i) / SPRITE_COUNT) + 4;
      // expand collision box to direct knockback away from boss
      s->w = ABS(mCenter.x - (s->x + 16)) + 16;
      s->cx = -s->w + 16 + (mCenter.x - ((TInt)s->x + 16)) / 2;
      s->h = ABS(mCenter.y - (s->y + 4)) + 8;
      s->cy = MAX(4, mCenter.y - (s->y));
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
