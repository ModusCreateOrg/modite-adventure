#include <GResources.h>
#include "GPlayerBulletProcess.h"
#include "GPlayer.h"

const TInt VELOCITY = 4;

class BulletSprite : public GAnchorSprite {
public:
  BulletSprite(GGameState *aGameState) : GAnchorSprite(aGameState, 0, PLAYER_SLOT) {
    Name("PLAYER BULLET");
    type = STYPE_PBULLET;
    SetCMask(STYPE_ENEMY | STYPE_OBJECT);
    SetFlags(SFLAG_CHECK);
    ClearFlags(SFLAG_RENDER | SFLAG_ANCHOR);
  }
};

GPlayerBulletProcess::GPlayerBulletProcess(GGameState *aGameState, DIRECTION aDirection)
    : GProcess(ATTR_GONE) {
  mDirection = aDirection;
  mSprite = new BulletSprite(aGameState);
  mSprite->type = STYPE_PBULLET;
  mSprite->mHitStrength = GPlayer::mHitStrength;
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->w = 30;
      mSprite->h = 8;
      mSprite->vy = -VELOCITY;
      mRange = 48;
      break;
    case DIRECTION_DOWN:
      mSprite->w = 30;
      mSprite->h = 8;
      mSprite->vy = VELOCITY;
      mRange = 30;
      break;
    case DIRECTION_LEFT:
      mSprite->w = 8;
      mSprite->h = 26;
      mSprite->vx = -VELOCITY;
      mSprite->cy = -8;
      mRange = 40;
      break;
    case DIRECTION_RIGHT:
      mSprite->w = 8;
      mSprite->h = 26;
      mSprite->vx = VELOCITY;
      mSprite->cy = -8;
      mRange = 40;
      break;
    default:
      mRange = 0;
      break;
  }

  mSprite->x = GPlayer::mSprite->x - mSprite->w / 2 + 32;
  mSprite->y = GPlayer::mSprite->y - mSprite->h / 2 - 8;
  mSprite->mDirection = mDirection;
  mAge = 0;
  aGameState->AddSprite(mSprite);
}

GPlayerBulletProcess::~GPlayerBulletProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPlayerBulletProcess::RunBefore() {
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
  if (!mSprite->mGameState->mGamePlayfield->IsFloor(mSprite->x + mSprite->w / 2, mSprite->y + mSprite->h / 2)) {
    return EFalse;
  }
  if (mSprite->TestAndClearCType(STYPE_ENEMY)) {
    return EFalse;
  }
  mAge++;
  if (mAge * VELOCITY > mRange) {
    return EFalse;
  }
  mSprite->ClearCType(STYPE_OBJECT);
  return ETrue;
}
