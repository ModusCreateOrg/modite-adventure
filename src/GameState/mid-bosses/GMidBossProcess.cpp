#include "GMidBossProcess.h"
#include "GPlayer.h"
#include "GStatProcess.h"
#include "GMidBossDeathProcess.h"
#include "GSpellOverlayProcess.h"

// see https://github.com/ModusCreateOrg/modite-adventure/wiki/Mid-Boss-Design-Guidelines

const TFloat VELOCITY = 1.0;
const TInt BOUNCE_TIME = 10; // bounce around for 10 seconds

GMidBossProcess::GMidBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot) : BProcess() {
  mSprite = ENull;
  mGameState = aGameState;
  mPlayfield = mGameState->mGamePlayfield;
  mStartX = aX;
  mStartY = aY;

  mSprite = new GAnchorSprite(mGameState, ENEMY_PRIORITY, aSlot, 0, STYPE_ENEMY);
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->cx = 20;
  mSprite->cy = 0;
  mSprite->w = 44;
  mSprite->h = 75;
  // This might not work if the sprite positions of the mid boss bitmaps are radically different from one another 
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(MID_BOSS_FIRE_BMP_SPRITES);
  mGameState->AddSprite(mSprite);
  mSprite->mHitPoints = 45;
  mSprite->mHitStrength = HIT_HARD;
  mDeathCounter = 0;
  mSpellCounter = 0;

  gEventEmitter.Listen(EVENT_SPELL_PROCESS_EXIT, this);
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
    case MB_MOVE_STATE:
      return MoveState();
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
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      mStateTimer = Random(15, 180);
      Idle(aDirection);
      break;

    case MB_BALL_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Ball(aDirection);
      break;

    case MB_MOVE_STATE:
      mStep = 0;
      mSprite->x += 32;
      mSprite->y -= 32;
      mStateTimer = BOUNCE_TIME * FRAMES_PER_SECOND;
      mSprite->type = STYPE_EBULLET;
      mSprite->SetFlags(SFLAG_CHECK);
      Move(aDirection);
      break;

    case MB_RETURN_STATE:
      mStep = 0;
      // set velocities toward starting position
      // will take 60 frames (1 second) to return to start
      mSprite->vx = (mStartX - mSprite->x) / 60;
      mSprite->vy = (mStartY - mSprite->y) / 60;
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      Return(aDirection);
      break;

    case MB_REVERT_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      Revert(aDirection);
      break;

    case MB_WALK_STATE:
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      mStep = 1 - mStep;
      mStateTimer = Random(30, 270);
      Walk(aDirection);
      break;

    case MB_ATTACK_STATE:
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mAttackTimer = MID_BOSS_ATTACK_TIME;
      Attack(aDirection);
      break;

    case MB_HIT_STATE:
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->ClearCMask(STYPE_EBULLET);
      Hit(aDirection);
      break;

    case MB_SPELL_STATE:
      mSprite->cx = 20;
      mSprite->cy = 0;
      mSprite->w = 44;
      mSprite->h = 75;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->ClearCMask(STYPE_EBULLET);
      Spell(aDirection);
      {
        mSpellCounter += 2;
        auto *p = new GSpellOverlayProcess(mGameState, mSprite->x + 44, mSprite->y - 75 + 32);
        mSpellOverlayProcess = p;
        mGameState->AddProcess(p);
        p = new GSpellOverlayProcess(mGameState, mSprite->x + 44, mSprite->y - 75 + 64);
        mGameState->AddProcess(p);
      }
      Hit(mSprite->mDirection);
      break;

    case MB_DEATH_STATE:
      Death(aDirection);
      {
        // get coordinates for explosion placement
        TRect r;
        mSprite->GetRect(r);
        r.Dump();
        mDeathCounter = 10;
        for (TInt delay = 0; delay < mDeathCounter; delay++) {
          printf("DEATH SPRITE @ %d,%d\n", r.x1, r.x2);
          auto *p = new GMidBossDeathProcess(mGameState, this, r.x1, r.y1, delay);
          mGameState->AddProcess(p);
        }
      }
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

TBool GMidBossProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 0) {
    if ((Random() & 7) == 0) {
      NewState(MB_BALL_STATE, DIRECTION_DOWN);
      return ETrue;
    }

    // TODO: rewrite this logic.  It doesn't need to loop.  It can try random direction, then the opposite
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
  if (mSprite->AnimDone()) {
    NewState(MB_MOVE_STATE, DIRECTION_DOWN);
  }
  return ETrue;
}

TBool GMidBossProcess::MaybeBounce() {
  TFloat vx = mSprite->vx, vy = mSprite->vy;
  TRect r;
  mSprite->GetRect(r);
  r.Offset(-vx, -vy);

  TBool bouncedX = EFalse, bouncedY = EFalse;

  if (vx > 0) {
    // check right edge (upper right, lower right corners)
    if (!mSprite->IsFloorTile(mSprite, r.x2, r.y1 + 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
    else if (!mSprite->IsFloorTile(mSprite, r.x2, r.y2 - 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
  }
  else {
    // check left edge (upper left, lower left corners)
    if (!mSprite->IsFloorTile(mSprite, r.x1, r.y1 + 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
    else if (!mSprite->IsFloorTile(mSprite, r.x1, r.y2 - 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
  }

  if (vy > 0) {
    // check bottom edge (lower left, lower right corners)
    if (!mSprite->IsFloorTile(mSprite, r.x1, r.y2)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
    if (!mSprite->IsFloorTile(mSprite, r.x2, r.y2)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
  }
  else {
    // check top edge (upper left, upper right corners)
    if (!mSprite->IsFloorTile(mSprite, r.x1, r.y1)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
    if (!mSprite->IsFloorTile(mSprite, r.x2, r.y1)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
  }

  return bouncedX || bouncedY;
}

TBool GMidBossProcess::MoveState() {
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET); // invulnerable

  if (--mStateTimer <= 0) {
    NewState(MB_RETURN_STATE, DIRECTION_UP);
    return ETrue;
  }
  MaybeBounce();
  return ETrue;
}

TBool GMidBossProcess::ReturnState() {
  TFloat dx = (mSprite->x - mStartX),
         dy = (mSprite->y - mStartY);

  if (SQRT(dx * dx + dy * dy) < 5) {
    mSprite->x = mStartX;
    mSprite->y = mStartY;
    mSprite->vx = mSprite->vy = 0.0;
    NewState(MB_REVERT_STATE, DIRECTION_DOWN);
  }
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET); // invulnerable
  return ETrue;
}

TBool GMidBossProcess::RevertState() {
  if (mSprite->AnimDone()) {
    if (mSprite->TestCType(STYPE_PLAYER)) {
      mSprite->type = STYPE_EBULLET;
    }
    else {
      mSprite->type = STYPE_ENEMY;
    }
    mSprite->SetFlags(SFLAG_CHECK);
    mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET);
    mSprite->SafePosition(GPlayer::mSprite);
    NewState(MB_IDLE_STATE, DIRECTION_DOWN);
  }
  return ETrue;
}

TBool GMidBossProcess::AttackState() {
  return ETrue;
}

TBool GMidBossProcess::HitState() {
  if (mSprite->AnimDone()) {
    if (mSprite->mHitPoints <= 0) {
      NewState(MB_DEATH_STATE, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      NewState(MB_IDLE_STATE, mSprite->mDirection);
    }
  }
  return ETrue;
}

TBool GMidBossProcess::DeathState() {
  if (mDeathCounter <= 3) {
    return EFalse;
  }
  return ETrue;
}

TBool GMidBossProcess::SpellState() {
  while (BEventMessage *m = GetMessage()) {
    if (m->mType == EVENT_SPELL_PROCESS_EXIT) {
      mSpellCounter--;
    }
  }
  if (mSprite->AnimDone() && mSpellCounter <= 0) {
    if (mSprite->mHitPoints <= 0) {
      NewState(MB_DEATH_STATE, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      NewState(MB_IDLE_STATE, mSprite->mDirection);
    }
  }
  return ETrue;
}
