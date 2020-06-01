#ifndef MODITE_GGRUNTPROCESS_H
#define MODITE_GGRUNTPROCESS_H

#include <BMemoryStream.h>
#include "Game.h"
#include "GPlayerProcess.h"
#include "GEnemyProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GEnemySprite.h"
#include "common/GSpellOverlayProcess.h"
#include "GameState/enemies/grunts/GEnemyDeathOverlayProcess.h"

const TInt16 ATTACK_TIME = 1 * FRAMES_PER_SECOND;

enum {
  IDLE_STATE,
  WALK_STATE,
  ATTACK_STATE,
  HIT_STATE,
  DEATH_STATE,
  SPELL_STATE, // hit with magic spell
  TAUNT_STATE,
};

static const char *stateMessages[] = {
  "IDLE STATE",
  "WALK STATE",
  "ATTACK STATE",
  "HIT STATE",
  "DEATH STATE",
  "SPELL STATE",
  "TAUNT STATE",
};

class GGruntProcess : public GEnemyProcess {
public:
  GGruntProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aParams, TFloat aVelocity, TUint16 aAttribute);

  ~GGruntProcess() OVERRIDE;

protected:
  TInt TauntTime() { return Random(4, 8) * FRAMES_PER_SECOND; }

protected:
  TInt mIp;
  TUint16 mParams;
  GSpellOverlayProcess *mSpellOverlayProcess;
  GEnemyDeathOverlayProcess *mEnemyDeathOverlayProcess;
  TInt16 mAttackTimer;
  TInt16 mStateTimer;
  TFloat mVelocity;
  TInt mRangeX, mRangeY;

  TBool mTaunt;
  TInt16 mTauntTimer;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

  // sfx
protected:
  static void SfxTakeDamage() {}
  static void SfxDeath() {
    gSoundPlayer.TriggerSfx(SFX_ENEMY_DEATH_WAV, 5);
  }
  static void SfxTaunt() {
//    gSoundPlayer.TriggerSfx(SFX_ENEMY_TAUNT_WAV);
//    printf("Enemy Taunt\n");

  }
  static void SfxAttack() {
    gSoundPlayer.TriggerSfx(SFX_ENEMY_ATTACK_WAV);
  }

protected:
  // test if a wall in the specified direction from sprite's current location
  TBool IsWallInDirection(DIRECTION aDirection);

  // test if enemy can walk in specified direction at the specified velocity
  // enemy can override this for more custom kinds of logic
  virtual TBool CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat aVy);

protected:
  virtual TBool MaybeAttack();

  TBool MaybeHit();

  TBool MaybeTaunt();

protected:
  virtual void NewState(TUint16 aState, DIRECTION aDirection);

  virtual void Idle(DIRECTION aDirection) = 0;
  TBool IdleState();

  virtual void Taunt(DIRECTION aDirection) = 0;
  TBool TauntState();

  virtual void Walk(DIRECTION aDirection) = 0;
  TBool WalkState();

  virtual void Attack(DIRECTION aDirection) = 0;
  TBool AttackState();

  virtual void Hit(DIRECTION aDirection) = 0;
  virtual TBool HitState();

  virtual void Death(DIRECTION aDirection) = 0;
  TBool DeathState();

  virtual void Spell(DIRECTION aDirection) = 0;
  virtual TBool SpellState();

  void OverlayAnimationComplete() OVERRIDE;

public:
  static TInt16 mCount; // number of enemy processes

public:
  void WriteToStream(BMemoryStream &aStream) OVERRIDE;
  void ReadFromStream(BMemoryStream &aStream) OVERRIDE;
};

#endif //MODITE_GGRUNTPROCESS_H
