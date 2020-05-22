#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

class GGamePlayfield;
class GGameState;
class BMemoryStream;

//class GAnchorSprite;

#include "Game.h"
#include "GPlayerSprite.h"
#include "GProcess.h"
#include "GGame.h"

const TFloat PLAYER_VELOCITY = 3 / TFloat(FACTOR);
const TFloat PLAYER_FRICTION = 0.5 / TFloat(FACTOR);
const TInt FALL_DURATION = 10 * FACTOR;
const TInt CHARGE_START_DELAY = 0.6 * FRAMES_PER_SECOND;
const TInt CHARGE_DURATION = 1.2 * FRAMES_PER_SECOND;
const TInt PERFECT_CHARGE_WINDOW = 2 * FACTOR; // perfect charge window duration in frames
const TFloat CHARGE_BONUS = 2.0; // damage multiplier for maximum charged sword
const TFloat PERFECT_CHARGE_BONUS = 3.5; // damage multiplier for perfect charge

class GPlayerProcess : public GProcess {
public:
  EXPLICIT GPlayerProcess(GGameState *aGameState);

  ~GPlayerProcess() OVERRIDE;

private:
  void StartBlink(TUint16 aTime) {
    mBlinkTimer = aTime;
  }

public:
  void StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY, TInt16 aExitingDungeon = OVERWORLD_DUNGEON, TInt16 aExitingLevel = -1);

  GAnchorSprite *Sprite() { return mSprite; }

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  TBool IsLedge();

  TBool CanWalk(TFloat aVx, TFloat aVy);

  void StartKnockback();

protected:
  void NewState(TUint16 aState, DIRECTION aDirection);

  TBool MaybeHit();

  DIRECTION MaybeMove(TFloat aSpeed);
  TBool MaybeWalk();

  TBool MaybeSword();

  TBool MaybeFall();

  TBool MaybeSpell();

protected:
  TBool IdleState();

  TBool SpellState();

  TBool WalkState();

  TBool SwordState();

  TBool FallState();

  TBool HitState();

public:
  void WriteCustomToStream(BMemoryStream &aStream) OVERRIDE;
  void ReadCustomFromStream(BMemoryStream &aStream) OVERRIDE;

public:
  static DIRECTION mLastDirection;
//  static TUint16 mLastDirection;
  static TFloat mRespawnAt[2];

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
//  GAnchorSprite *mSprite, *mSprite2;
  GPlayerSprite *mSprite;
  TUint16 mState;
  TUint16 mStep;
  TUint16 mStepFrame;
  TUint16 mBlinkTimer;
};

#endif // MODITE_GPLAYERPROCESS_H
