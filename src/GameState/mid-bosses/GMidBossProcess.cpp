#include "GMidBossProcess.h"

// see https://github.com/ModusCreateOrg/modite-adventure/wiki/Mid-Boss-Design-Guidelines

const TInt ATTACK_TIME = 3 * FRAMES_PER_SECOND;

GMidBossProcess::GMidBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot) : BProcess() {
  mSprite = ENull;
  mGameState = aGameState;
  mPlayfield = mGameState->mGamePlayfield;
  mStartX = aX;
  mStartY = aY;


  mSprite = new GAnchorSprite(mGameState, ENEMY_PRIORITY, aSlot, 0, STYPE_ENEMY);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 44;
  mSprite->h = 75;
  mGameState->AddSprite(mSprite);
}

GMidBossProcess::~GMidBossProcess() {
  if (mSprite) {
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GMidBossProcess::RunBefore() {
  if (mSprite->Clipped()) {
    NewState(MB_IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  switch (mState) {
    case MB_IDLE_STATE:
      return IdleState();
    case MB_BALL_STATE:
      return BallState();
    case MB_RETURN_STATE:
      return ReturnState();
    case MB_REVERT_STATE:
      return RevertState();
    case MB_WALK_STATE:
      return WalkState();
    case MB_ATTACK_STATE:
      return AttackState();
    case MB_HIT_STATE:
      return HitState();
    case MB_DEATH_STATE:
      return DeathState();
    case MB_SPELL_STATE:
      return SpellState();
    default:
      return ETrue;
  }
}

TBool GMidBossProcess::RunAfter() {
  return ETrue;
}

void GMidBossProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;

  switch (aState) {

    case MB_IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Idle(aDirection);
      break;

    case MB_BALL_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Ball(aDirection);
      break;

    case MB_RETURN_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Return(aDirection);
      break;

    case MB_REVERT_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Revert(aDirection);
      break;

    case MB_WALK_STATE:
      mStep = 1 - mStep;
      Walk(aDirection);
      break;

    case MB_ATTACK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mAttackTimer = ATTACK_TIME;
      Attack(aDirection);
      break;

    case MB_HIT_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->cMask &= ~STYPE_EBULLET;
      Hit(aDirection);
      break;

    case MB_SPELL_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->cMask &= ~STYPE_EBULLET;
      Spell(aDirection);
      break;

    case MB_DEATH_STATE:
      //      Death(aDirection);
      break;

    default:
      break;
  }
}

TBool GMidBossProcess::IdleState() {
  return ETrue;
}

TBool GMidBossProcess::WalkState() {
  return ETrue;
}

TBool GMidBossProcess::BallState() {
  return ETrue;
}

TBool GMidBossProcess::ReturnState() {
  return ETrue;
}

TBool GMidBossProcess::RevertState() {
  return ETrue;
}

TBool GMidBossProcess::AttackState() {
  return ETrue;
}

TBool GMidBossProcess::HitState() {
  return ETrue;
}

TBool GMidBossProcess::DeathState() {
  return ETrue;
}

TBool GMidBossProcess::SpellState() {
  return ETrue;
}

