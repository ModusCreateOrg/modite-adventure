#include "GMidBossProcess.h"
#include "GItemProcess.h"
#include "GMidBossDeathProcess.h"
#include "GPlayer.h"
#include "common/GSpellOverlayProcess.h"
#include "Resources.h"
#include "GGame.h"

// see
// https://github.com/ModusCreateOrg/modite-adventure/wiki/Mid-Boss-Design-Guidelines

GMidBossProcess::GMidBossProcess(GGameState *aGameState, TFloat aX, TFloat aY,
                                 TUint16 aSlot, TInt aIp, TUint16 aAttribute,
                                 TUint16 aDropsItemAttribute,
                                 TInt16 aSpriteSheet)
    : GBossProcess(aGameState, aX, aY, aSlot, aAttribute) {
  mIp = aIp;
  mSaveToStream = ETrue;
  mDropsItemAttribute = aDropsItemAttribute;

  mSprite->cx = 20;
  mSprite->cy = 8;
  mSprite->w = 44;
  mSprite->h = 24;
  // This might not work if the sprite positions of the mid boss bitmaps are
  // radically different from one another
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  SetStatMultipliers(8.0, 3.0, 10.0);
  mSpellOverlayProcess = ENull;
  switch (aAttribute) {
  case ATTR_MID_BOSS_WATER:
    mSprite->mElement = ELEMENT_WATER;
    mSprite->Name("Iuy");
    break;
  case ATTR_MID_BOSS_FIRE:
    mSprite->mElement = ELEMENT_FIRE;
    mSprite->Name("Ustir");
    break;
  case ATTR_MID_BOSS_EARTH:
    mSprite->mElement = ELEMENT_EARTH;
    mSprite->Name("Oim");
    break;
  case ATTR_MID_BOSS_ENERGY:
    mSprite->mElement = ELEMENT_ENERGY;
    mSprite->Name("Igum");
    break;
  default:
    break;
  }

  //  gEventEmitter.Listen(EVENT_SPELL_PROCESS_EXIT, this);
}

GMidBossProcess::~GMidBossProcess() {
  auto *gameState = (GGameState*)gGameEngine;
  gameState->PlayMusicForCurrentLevel();
}

TBool GMidBossProcess::RunBefore() {
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
  case MB_CHARGE_STATE:
    return ChargeState();
  case MB_DEATH_STATE:
    return DeathState();
  case MB_SPELL_STATE:
    return SpellState();
  default:
    return ETrue;
  }
}

TBool GMidBossProcess::RunAfter() {
  UpdateBlink();
  if (mHitTimer-- < 0) {
    mHitTimer = 0;
  }
  return ETrue;
}

void GMidBossProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->type = STYPE_ENEMY;

  switch (aState) {

  case MB_IDLE_STATE:
    mStep = 0;
    mSprite->vx = 0;
    mSprite->vy = 0;
    mStateTimer = Random(15, 180);
    mSprite->type = STYPE_ENEMY;
    Idle(aDirection);
    mSprite->ResetShadow();
    break;

  case MB_BALL_STATE:
    mStep = 0;
    mSprite->vx = 0;
    mSprite->vy = 0;
    Ball(aDirection);
    mSprite->ClearFlags(SFLAG_KNOCKBACK);
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_TRANSFORM_IN_WAV);
    break;

  case MB_MOVE_STATE:
    mStep = 0;
    mStateTimer = BOUNCE_TIME * FRAMES_PER_SECOND;
    mSprite->type = STYPE_EBULLET;
    mSprite->SetFlags(SFLAG_CHECK);
    Move(aDirection);
    mSprite->ResetShadow();
    break;

  case MB_RETURN_STATE:
    mStep = 0;
    // set velocities toward starting position
    // will take 60 frames (1 second) to return to start
    mSprite->vx = (mStartX - mSprite->x) / 60;
    mSprite->vy = (mStartY - mSprite->y) / 60;
    Return(aDirection);
    break;

  case MB_REVERT_STATE:
    mStep = 0;
    mSprite->vx = 0;
    mSprite->vy = 0;
    Revert(aDirection);
    mSprite->ResetShadow();
    mSprite->SetFlags(SFLAG_KNOCKBACK);
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_TRANSFORM_OUT_WAV);

    break;

  case MB_WALK_STATE:
    mStep = 1 - mStep;
    mStateTimer = Random(30, 270);
    Walk(aDirection);
    break;

  case MB_CHARGE_STATE:
    mStep = 0;
    mSprite->vx = 0;
    mSprite->vy = 0;
    mStateTimer = -FRAMES_PER_SECOND;
    Charge(aDirection);
    break;

  case MB_ATTACK_STATE:
    mSprite->vx = 0;
    mSprite->vy = 0;
    mStep = 0;
    mAttackTimer = MID_BOSS_ATTACK_TIME;
    Attack(aDirection);
    break;

  case MB_SPELL_STATE:
    mSprite->vx = 0;
    mSprite->vy = 0;
    mStep = 0;
    mSprite->ClearCMask(STYPE_EBULLET);
    Spell(aDirection);
    {
      mSpellCounter += 2;
      auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 22,
                                         mSprite->y + 1);
      mSpellOverlayProcess = p;
      mGameState->AddProcess(p);
      p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 44,
                                   mSprite->y + 1);
      mGameState->AddProcess(p);
    }
    break;

  case MB_DEATH_STATE:
    mSprite->vx = 0;
    mSprite->vy = 0;
    mSprite->type = STYPE_OBJECT;
    Death(aDirection);
    {
      // get coordinates for explosion placement
      TRect r;
      mSprite->GetRect(r);
      //        r.Dump();
      mDeathCounter = 10;
      for (TInt delay = 0; delay < mDeathCounter; delay++) {
        printf("DEATH SPRITE @ %d,%d\n", r.x1, r.x2);
        auto *p =
            new GMidBossDeathProcess(mGameState, this, r.x1, r.y1 - 64, delay);
        mGameState->AddProcess(p);
      }
    }
  default:
    break;
  }
}

TBool GMidBossProcess::MaybeHit() {
  if (SpellDamageCheck()) {
    NewState(MB_SPELL_STATE, mSprite->mDirection);
    mHitTimer += HIT_SPAM_TIME * 2;
    return ETrue;
  }

  if (BasicDamageCheck()) {
    StartBlink(BLINK_DURATION);
    mHitTimer += HIT_SPAM_TIME;
    return ETrue;
  }

  return EFalse;
}

TBool GMidBossProcess::MaybeDeath() {
  if (mHitPoints <= 0) {
    NewState(MB_DEATH_STATE, mSprite->mDirection);
    return ETrue;
  }
  return EFalse;
}

TBool GMidBossProcess::IdleState() {
  MaybeHit();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (mHitTimer > HIT_SPAM_TIME * 3) {
    NewState(MB_BALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }

  if (--mStateTimer < 0) {
    if (!(Random() % 10)) {
      NewState(MB_BALL_STATE, DIRECTION_DOWN);
      return ETrue;
    }
    if (!(Random() % 3)) {
      NewState(MB_CHARGE_STATE, mSprite->mDirection);
      return ETrue;
    }

    DIRECTION direction = GAnchorSprite::RandomDirection();
    TFloat vx = direction == DIRECTION_LEFT ? -VELOCITY : direction == DIRECTION_RIGHT ? VELOCITY : 0,
           vy = direction == DIRECTION_UP ? -VELOCITY : direction == DIRECTION_DOWN ? VELOCITY : 0;

    if (mSprite->CanWalk(vx, vy)) {
      NewState(MB_WALK_STATE, direction);
      return ETrue;
    }

    NewState(MB_IDLE_STATE, mSprite->mDirection);
  }

  if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
    mSprite->Nudge();
    return ETrue;
  }

  return ETrue;
}

TBool GMidBossProcess::WalkState() {
  MaybeHit();

  if (MaybeDeath()) {
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

  if (!mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
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
    if (!mSprite->IsFloorTile(r.x2, r.y1 + 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    } else if (!mSprite->IsFloorTile(r.x2, r.y2 - 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
  } else {
    // check left edge (upper left, lower left corners)
    if (!mSprite->IsFloorTile(r.x1, r.y1 + 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    } else if (!mSprite->IsFloorTile(r.x1, r.y2 - 8)) {
      mSprite->vx = -vx;
      bouncedX = ETrue;
      mSprite->x = mSprite->mLastX;
      mSprite->y = mSprite->mLastY;
      mSprite->GetRect(r);
    }
  }

  if (vy > 0) {
    // check bottom edge (lower left, lower right corners)
    if (!mSprite->IsFloorTile(r.x1, r.y2)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
    if (!mSprite->IsFloorTile(r.x2, r.y2)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
  } else {
    // check top edge (upper left, upper right corners)
    if (!mSprite->IsFloorTile(r.x1, r.y1)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
    if (!mSprite->IsFloorTile(r.x2, r.y1)) {
      mSprite->vy = -vy;
      bouncedY = ETrue;
    }
  }

  if (bouncedX || bouncedY) {
    gSoundPlayer.TriggerSfx(SFX_MIDBOSS_BOUNCE_WALL_WAV);
  }

  return bouncedX || bouncedY;
}

TBool GMidBossProcess::MoveState() {
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET |
                      STYPE_SPELL); // invulnerable

  if (--mStateTimer <= 0) {
    NewState(MB_RETURN_STATE, DIRECTION_UP);
    return ETrue;
  }
  MaybeBounce();
  return ETrue;
}

TBool GMidBossProcess::ReturnState() {
  TFloat dx = (mSprite->x - mStartX), dy = (mSprite->y - mStartY);

  if (SQRT(dx * dx + dy * dy) < 5) {
    mSprite->x = mStartX;
    mSprite->y = mStartY;
    mSprite->vx = mSprite->vy = 0.0;
    NewState(MB_REVERT_STATE, DIRECTION_DOWN);
  }
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET |
                      STYPE_SPELL); // invulnerable
  return ETrue;
}

TBool GMidBossProcess::RevertState() {
  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
  }
  if (mSprite->AnimDone()) {
    if (mSprite->TestCType(STYPE_PLAYER)) {
      mSprite->type = STYPE_EBULLET;
    } else {
      mSprite->type = STYPE_ENEMY;
    }
    mSprite->SetFlags(SFLAG_CHECK);
    mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET |
                        STYPE_SPELL); // invulnerable
    mSprite->SafePosition(GPlayer::mSprite);
    NewState(MB_IDLE_STATE, DIRECTION_DOWN);
  }
  return ETrue;
}

TBool GMidBossProcess::AttackState() { return ETrue; }

TBool GMidBossProcess::ChargeState() {
  MaybeHit();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (mStateTimer < 0) {
    mStateTimer++;
    TFloat xx = GPlayer::mSprite->x - mSprite->x - 32,
           yy = GPlayer::mSprite->y - mSprite->y - 4;
    DIRECTION direction;
    if (yy < xx) {
      direction = yy < -xx ? DIRECTION_UP : DIRECTION_RIGHT;
    } else {
      direction = yy < -xx ? DIRECTION_LEFT : DIRECTION_DOWN;
    }
    if (mSprite->AnimDone() || mSprite->mDirection != direction) {
      mSprite->mDirection = direction;
      Charge(direction);
    }
    if (mStateTimer == 0) {
      mSprite->vx = CHARGE_VELOCITY * xx / hypot(xx, yy);
      mSprite->vy = CHARGE_VELOCITY * yy / hypot(xx, yy);
      mSprite->type = STYPE_EBULLET;
    }
  } else if (mStateTimer == 0) {
    if (mSprite->AnimDone()) {
      Charge(mSprite->mDirection);
    }
    if (MaybeBounce()) {
      mStateTimer++;
      mSprite->vx *= VELOCITY / CHARGE_VELOCITY;
      mSprite->vy *= VELOCITY / CHARGE_VELOCITY;
    }
    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      NewState(MB_IDLE_STATE, mSprite->mDirection);
    }
  } else if (mStateTimer < HOP_DURATION) {
    mStateTimer++;
    mSprite->mDy = GRAVITY * .5 * mStateTimer * (mStateTimer - HOP_DURATION);
    MaybeBounce();
  } else if (mStateTimer == HOP_DURATION) {
    Land(mSprite->mDirection);
    mStateTimer++;
    MaybeBounce();
  } else {
    mSprite->vx *= 1 - MID_BOSS_FRICTION;
    mSprite->vy *= 1 - MID_BOSS_FRICTION;
    if (mSprite->AnimDone()) {
      NewState(MB_IDLE_STATE, mSprite->mDirection);
    }
    MaybeBounce();
  }
  mSprite->TestAndClearCType(STYPE_PLAYER);


  return ETrue;
}

TBool GMidBossProcess::DeathState() {
  if (mDeathCounter == 1) {
    printf("MID BOSS DEATH\n");
  }
  if (mDeathCounter <= 3) {
    printf("drop $%x %d\n", mDropsItemAttribute, mDropsItemAttribute);
    GItemProcess::SpawnItem(mGameState, mIp, mDropsItemAttribute,
                            GPlayer::mSprite->x + 32, GPlayer::mSprite->y);
    return EFalse;
  }
  // maybe drop item
  return ETrue;
}

TBool GMidBossProcess::SpellState() {
  //  while (BEventMessage *m = GetMessage()) {
  //    if (m->mType == EVENT_SPELL_PROCESS_EXIT) {
  //      mSpellCounter--;
  //    }
  //  }
  if (mSprite->AnimDone() && mSpellCounter <= 0) {
    if (mHitPoints <= 0) {
      NewState(MB_DEATH_STATE, mSprite->mDirection);
    } else {
      mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      NewState(MB_IDLE_STATE, mSprite->mDirection);
    }
  }
  return ETrue;
}

void GMidBossProcess::OverlayAnimationComplete() { mSpellCounter--; }

void GMidBossProcess::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mIp, sizeof(mIp));
  aStream.Write(&mStartX, sizeof(mStartX));
  aStream.Write(&mStartY, sizeof(mStartY));
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
  aStream.Write(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Write(&mStateTimer, sizeof(mStateTimer));
  mSprite->WriteToStream(aStream);
}

void GMidBossProcess::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mIp, sizeof(mIp));
  aStream.Read(&mStartX, sizeof(mStartX));
  aStream.Read(&mStartY, sizeof(mStartY));
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
  aStream.Read(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Read(&mStateTimer, sizeof(mStateTimer));
  mSprite->ReadFromStream(aStream);
}
