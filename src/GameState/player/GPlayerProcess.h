#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

class GGameState;
class GGamePlayfield;
class GGameState;
class GPlayerBlinkProcess;
class BMemoryStream;

//class GAnchorSprite;

#include "GAnchorSprite.h"
#include "GProcess.h"
#include "GGame.h"

const TFloat PLAYER_VELOCITY = 3 / TFloat(FACTOR);

class GPlayerProcess : public GProcess {
public:
  GPlayerProcess(GGameState *aGameState);

  ~GPlayerProcess();

public:
  void StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  GAnchorSprite *Sprite() { return mSprite; }

  TFloat PlayerX();

  TFloat PlayerY();

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
  TBool MaybeSpell();

protected:
  TBool IdleState();

  TBool QuaffState();

  TBool SpellState();

  TBool WalkState();

  TBool SwordState();

  TBool FallState();

  TBool HitState();

public:
  void WriteToStream(BMemoryStream &aStream);
  void WriteCustomToStream(BMemoryStream &aStream);

  void ReadFromStream(BMemoryStream &aStream);
  void ReadCustomFromStream(BMemoryStream &aStream);

public:
  GPlayerBlinkProcess *mBlinkProcess;

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
//  GAnchorSprite *mSprite, *mSprite2;
  TUint16 mState;
  TUint16 mStep;
};

#endif // MODITE_GPLAYERPROCESS_H
