#ifndef MODITE_GENEMYPROCESS_H
#define MODITE_GENEMYPROCESS_H

#include <BMemoryStream.h>
#include "GResources.h"
#include "GProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GEnemySprite.h"
#include "common/GSpellOverlayProcess.h"
#include "GEnemyDeathOverlayProcess.h"

const TInt16 ATTACK_TIME = 1 * FRAMES_PER_SECOND;

enum {
  IDLE_STATE,
  WALK_STATE,
  ATTACK_STATE,
  HIT_STATE,
  DEATH_STATE,
  SPELL_STATE, // hit with magic spell
};

static const char *stateMessages[] = {
  "IDLE STATE",
  "WALK STATE",
  "ATTACK STATE",
  "HIT STATE",
  "DEATH STATE",
  "SPELL STATE",
};

class GEnemyProcess : public GProcess {
public:
  GEnemyProcess(GGameState *aGameState, TInt aIp, TUint16 aSlot, TUint16 aParams, TFloat aVelocity, TUint16 aAttribute);

  ~GEnemyProcess() OVERRIDE;

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  TInt mIp;
  TUint16 mParams;
  GSpellOverlayProcess *mSpellOverlayProcess;
  GEnemyDeathOverlayProcess *mEnemyDeathOverlayProcess;
  TFloat mStartX, mStartY;
  TUint16 mState;
  TUint16 mDirection;
  TUint16 mStep;
  TInt16 mAttackTimer;
  TInt16 mStateTimer;
  TInt16 mHitPoints;
  TFloat mVelocity;
  TInt mRangeX, mRangeY;

  GAnchorSprite *mPlayerSprite;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

  // sfx
protected:
  virtual void SfxTakeDamage() {}
  virtual void SfxDeath() {}
  virtual void SfxTaunt() {}
  virtual void SfxAttack() {}

protected:
  // test if a wall in the specified direction from sprite's current location
  TBool IsWall(DIRECTION aDirection, TFloat aDx = 0.0, TFloat aDy = 0.0);

  // test if enemy can walk in specified direction at the specified velocity
  // enemy can override this for more custom kinds of logic
  virtual TBool CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy);

protected:
  TBool MaybeAttack();

  TBool MaybeHit();

  TBool MaybeTaunt();

protected:
  virtual void NewState(TUint16 aState, DIRECTION aDirection);

  virtual void Idle(DIRECTION aDirection) = 0;
  TBool IdleState();

  virtual void Walk(DIRECTION aDirection) = 0;
  TBool WalkState();

  virtual void Attack(DIRECTION aDirection) = 0;
  TBool AttackState();

  virtual void Hit(DIRECTION aDirection) = 0;
  TBool HitState();

  virtual void Death(DIRECTION aDirection) = 0;
  TBool DeathState();

  void Spell(DIRECTION aDirection);
  TBool SpellState();

  void OverlayAnimationComplete() OVERRIDE;

public:
  static TInt16 mCount; // number of enemy processes

public:
  void WriteToStream(BMemoryStream &aStream) OVERRIDE;
  void ReadFromStream(BMemoryStream &aStream) OVERRIDE;
};

#endif //MODITE_GENEMYPROCESS_H
