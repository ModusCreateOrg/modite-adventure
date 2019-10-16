#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

#include "Game.h"
#include "GAnchorSprite.h"
#include "GPlayer.h"

class GGamePlayfield;

const TFloat PLAYER_VELOCITY = 3 / TFloat(FACTOR);

class GPlayerProcess : public BProcess {
public:
  GPlayerProcess(GGameState *aGameState);

  ~GPlayerProcess();

public:
  void StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  GAnchorSprite *Sprite() { return mSprite; }

  TFloat PlayerX() { return mSprite->x; }

  TFloat PlayerY() { return mSprite->y; }

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  TBool IsLedge(TFloat aX, TFloat aY);

  TBool IsLedge();

  TBool CanWalk(DIRECTION aDirection);

protected:
  void NewState(TUint16 aState, DIRECTION aDirection);

  TBool MaybeHit();

  TBool MaybeWalk();

  TBool MaybeSword();

  TBool MaybeFall();

  TBool MaybeQuaff();

protected:
  TBool IdleState();

  TBool QuaffState();

  TBool WalkState();

  TBool SwordState();

  TBool FallState();

  TBool HitState();

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  GAnchorSprite *mSprite, *mSprite2;
  TUint16 mState;
  TUint16 mStep;
};

#endif // MODITE_GPLAYERPROCESS_H
