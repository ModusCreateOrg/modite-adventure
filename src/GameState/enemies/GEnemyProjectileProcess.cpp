#include <math.h>
#include "GEnemyProjectileProcess.h"

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_TIMEOUT = 90;
const TInt PROJECTILE_DAMAGE = 55;

GEnemyProjectileProcess::GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle)
    : GProcess(0, 0) {
  mSaveToStream = EFalse;
  mTimer = PROJECTILE_TIMEOUT;

  mSprite = new GAnchorSprite(aGameState, 0, BOSS_PROJECTILE_SLOT);
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->mHitStrength = PROJECTILE_DAMAGE;

  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 8;
  mSprite->h = 8;

  TUint8 angleIndex = 0;
  TFloat angle = 0;
  TFloat deltaMin = 99.;

  // top, bottom, left, right and angles in steps of 22.5 degrees
  TFloat angles[16] = {
    0,
    1.570796,
    -1.570796,
    M_PI,

    0.3926991,
    0.7853982,
    1.1780973,

    -0.3926991,
    -0.7853982,
    -1.1780973,

    1.9634954,
    2.3561944,
    2.7488935,

    -1.9634954,
    -2.3561944,
    -2.7488935,
  };

  // Find the closest angle to the player
  for (TUint8 i = 0; i < 16; i++) {
    const TFloat delta = ABS(aAngle - angles[i]);
    if (delta < deltaMin) {
      deltaMin = delta;
      angle = angles[i];
      angleIndex = i;
    }
  }

  // Adjust sprite based on angle
  switch (angleIndex) {
    case 0:
      mSprite->mImageNumber = 0;
      mSprite->y += 32;
      mSprite->cx = 20;
      mSprite->cy = -24;
      mSprite->SetFlags(SFLAG_RIGHT);
      break;
    case 1:
      mSprite->mImageNumber = 0;
      mSprite->x += 8;
      mSprite->y -= 32;
      mSprite->cx = -2;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLOP);
      break;
    case 2:
      mSprite->mImageNumber = 0;
      mSprite->cx = -2;
      mSprite->cy -= 24;
      break;
    case 3:
      mSprite->mImageNumber = 0;
      mSprite->cx = -4;
      mSprite->cy = 0;
      mSprite->SetFlags(SFLAG_LEFT);
      break;

    case 4:
      mSprite->mImageNumber = 3;
      mSprite->y -= 48;
      mSprite->cx = 18;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLOP);
      break;
    case 5:
      mSprite->mImageNumber = 2;
      mSprite->y -= 48;
      mSprite->cx = 15;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLOP);
      break;
    case 6:
      mSprite->mImageNumber = 1;
      mSprite->y -= 48;
      mSprite->cx = 8;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLOP);
      break;

    case 7:
      mSprite->mImageNumber = 3;
      mSprite->y += 16;
      mSprite->cx = 18;
      mSprite->cy -= 24;
      break;
    case 8:
      mSprite->mImageNumber = 2;
      mSprite->y += 16;
      mSprite->cx = 15;
      mSprite->cy -= 24;
      break;
    case 9:
      mSprite->mImageNumber = 1;
      mSprite->y += 16;
      mSprite->cx = 8;
      mSprite->cy -= 24;
      break;

    case 10:
      mSprite->mImageNumber = 1;
      mSprite->x -= 16;
      mSprite->y -= 48;
      mSprite->cx = 8;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLIP);
      mSprite->SetFlags(SFLAG_FLOP);
      break;
    case 11:
      mSprite->mImageNumber = 2;
      mSprite->x -= 16;
      mSprite->y -= 48;
      mSprite->cx = 2;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLIP);
      mSprite->SetFlags(SFLAG_FLOP);
      break;
    case 12:
      mSprite->mImageNumber = 3;
      mSprite->x -= 16;
      mSprite->y -= 48;
      mSprite->cx = 0;
      mSprite->cy = 64;
      mSprite->SetFlags(SFLAG_FLIP);
      mSprite->SetFlags(SFLAG_FLOP);
      break;

    case 13:
      mSprite->mImageNumber = 1;
      mSprite->x -= 16;
      mSprite->y += 16;
      mSprite->cx += 8;
      mSprite->cy -= 24;
      mSprite->SetFlags(SFLAG_FLIP);
      break;
    case 14:
      mSprite->mImageNumber = 2;
      mSprite->x -= 16;
      mSprite->y += 16;
      mSprite->cx += 1;
      mSprite->cy -= 24;
      mSprite->SetFlags(SFLAG_FLIP);
      break;
    case 15:
      mSprite->mImageNumber = 3;
      mSprite->x -= 16;
      mSprite->y += 16;
      mSprite->cy -= 24;
      mSprite->SetFlags(SFLAG_FLIP);
      break;
    default:
      Panic("Invalid angle index %d\n", angleIndex);
  }

  // Angles are in radians
  mSprite->vx = cos(angle) * PROJECTILE_VELOCITY;
  mSprite->vy = sin(angle) * PROJECTILE_VELOCITY;

  aGameState->AddSprite(mSprite);
  printf("ENEMY PROJECTILE at %f,%f with velocity of %f %f\n", mSprite->x, mSprite->y, mSprite->vx, mSprite->vy);
}

GEnemyProjectileProcess::~GEnemyProjectileProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyProjectileProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyProjectileProcess::RunAfter() {
  if (mSprite->Clipped()) {
    printf("ENEMY PROJECTILE CLIPPED\n");
    return EFalse;
  }
  if (!mSprite->IsFloor(DIRECTION_DOWN, mSprite->vx, mSprite->vy)) {
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
  }
  if (mSprite->vx < 0) {
    if (!mSprite->IsFloor(DIRECTION_LEFT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
    }
  }
  if (mSprite->vx > 0) {
    if (!mSprite->IsFloor(DIRECTION_RIGHT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
    }
  }
  if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
    printf("ENEMY PROJECTILE HIT PLAYER\n");
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
  }
 if (--mTimer < 1) {
    printf("ENEMY PROJECTILE TIMEOUT\n");
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    return EFalse;
  }

  return ETrue;
}
