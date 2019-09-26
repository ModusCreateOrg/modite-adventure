#include "GEnemyProcess.h"
#include "GPlayer.h"
#include "GGameState.h"

GEnemyProcess::GEnemyProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TUint16 aSlot) :
  mGameState(aGameState), mPlayfield(aGamePlayfield) {
  mState  = IDLE_STATE;
  mSprite = new GAnchorSprite(0, aSlot);
  mSprite->type  = STYPE_ENEMY;
  mSprite->cMask = STYPE_PLAYER | STYPE_PBULLET;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->w     = 32;
  mSprite->h     = 32;
  mSprite->Name("ENEMY SPRITE");
  mGameState->AddSprite(mSprite);
  mDirection = DIRECTION_DOWN;
  mState     = IDLE_STATE;
  mStep      = 0;

  mStartX       = mStartY = 0;
  mPlayerSprite = GPlayer::mSprite;
  mAttackTimer  = Random(30, 60);
}

GEnemyProcess::~GEnemyProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyProcess::IsWall(TInt aDirection) {
  const TInt leftX  = mSprite->x + mSprite->w / 2,
             rightX = mSprite->mRect.x2 - mSprite->w / 2, //  - leftX + mSprite->w - 1,
             top    = mSprite->y - mSprite->h,
             bottom = mSprite->y;

  switch (aDirection) {
    case DIRECTION_UP:
      if (mPlayfield->IsWall(leftX, top - 1)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX, top - 1)) {
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(leftX, bottom + 1)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX, bottom + 1)) {
        return ETrue;
      }
      break;
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(leftX - 8, top)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(leftX - 8, bottom)) {
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(rightX + 8, top)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX + 8, bottom)) {
        return ETrue;
      }
      break;
    default:
      Panic("GEnemyProcess::IsWall invalid direction %d\n", aDirection);
      break;
  }

  return EFalse;
}
