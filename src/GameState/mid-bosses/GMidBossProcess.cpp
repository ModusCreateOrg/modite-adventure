#include "GMidBossProcess.h"
#include "GPlayer.h"
#include "GStatProcess.h"
 

// see https://github.com/ModusCreateOrg/modite-adventure/wiki/Mid-Boss-Design-Guidelines

const TFloat VELOCITY = 1.0;

GMidBossProcess::GMidBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot) : BProcess() {
  mSprite = ENull;
  mGameState = aGameState;
  mPlayfield = mGameState->mGamePlayfield;
  mStartX = aX;
  mStartY = aY;

  mSprite = new GAnchorSprite(mGameState, ENEMY_PRIORITY, aSlot, 0, STYPE_ENEMY);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->cx = 20;
  mSprite->w = 44;
  mSprite->h = 75;
  mGameState->AddSprite(mSprite);
  mSprite->mHitPoints = 1000;
}

GMidBossProcess::~GMidBossProcess() {
  if (mSprite) {
    mSprite->Remove();
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
      mStateTimer = Random(15, 180);
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
      mStateTimer = Random(30, 270);
      Walk(aDirection);
      break;

    case MB_ATTACK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mAttackTimer = MID_BOSS_ATTACK_TIME;
      Attack(aDirection);
      break;

    case MB_HIT_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->ClearCMask(STYPE_EBULLET);
      Hit(aDirection);
      break;

    case MB_SPELL_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->ClearCMask(STYPE_EBULLET);
      Spell(aDirection);
      break;

    case MB_DEATH_STATE:
      //      Death(aDirection);
      break;

    default:
      break;
  }
}

TBool GMidBossProcess::MaybeHit() {
  if (mSprite->TestCType(STYPE_SPELL)) {
    mSprite->ClearCType(STYPE_SPELL);
    if (!mSprite->mInvulnerable) {
      mSprite->mInvulnerable = ETrue;
      // TODO take into account which spellbook is being wielded
      mSprite->mHitPoints -= GPlayer::mHitStrength;
      if (mSprite->mHitPoints <= 0) {
        printf("MID BOSS DEATH\n");
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      else {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "HIT +%d", GPlayer::mHitStrength));
      }
      NewState(MB_SPELL_STATE, mSprite->mDirection);
      return ETrue;
    }
  }

  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      mSprite->mHitPoints -= other->mHitStrength;
      if (mSprite->mHitPoints <= 0) {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      else {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "HIT +%d", other->mHitStrength));
      }
      switch (other->mDirection) {
        case DIRECTION_RIGHT:
          NewState(MB_HIT_STATE, DIRECTION_LEFT);
          break;
        case DIRECTION_LEFT:
          NewState(MB_HIT_STATE, DIRECTION_RIGHT);
          break;
        case DIRECTION_UP:
          NewState(MB_HIT_STATE, DIRECTION_DOWN);
          break;
        case DIRECTION_DOWN:
          NewState(MB_HIT_STATE, DIRECTION_UP);
          break;
      }
      mSprite->Nudge();
      return ETrue;
    }
  }

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
    return ETrue;
  }

  return EFalse;
}

//TBool GMidBossProcess::MaybeAttack() {
//  return EFalse;
//}

TBool GMidBossProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 0) {
    for (TInt retries = 0; retries < 8; retries++) {
      DIRECTION direction = (Random() & 2) ? DIRECTION_LEFT : DIRECTION_RIGHT;

      TFloat vx = direction == DIRECTION_LEFT ? -VELOCITY : VELOCITY;

      if (mSprite->CanWalk(direction, vx, 0)) {
        NewState(MB_WALK_STATE, direction);
        return ETrue;
      }
    }

    // after 8 tries, we couldn't find a direction to walk.
    NewState(MB_IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GMidBossProcess::WalkState() {
  if (MaybeHit()) {
    NewState(MB_IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  mAttackTimer = 1;

  if (--mStateTimer < 0) {
    NewState(MB_IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_PBULLET)) {
    NewState(MB_IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (!mSprite->CanWalk(mSprite->mDirection, mSprite->vx, mSprite->vy)) {
    NewState(MB_IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    Walk(mSprite->mDirection);
  }

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
  if (mSprite->AnimDone()) {
    if (mSprite->mHitPoints <= 0) {
      NewState(MB_DEATH_STATE, mSprite->mDirection);
      return ETrue;
    }
    mSprite->mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    NewState(MB_IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GMidBossProcess::DeathState() {
  return EFalse;
}

TBool GMidBossProcess::SpellState() {
  return ETrue;
}
 
    
    


  
  






 
    
    


  
  










  
  



    
        
                

     
    



  
  

  


     
      
      
      
      
     
  

    
      
        
      
      
      
        
      
      
    
