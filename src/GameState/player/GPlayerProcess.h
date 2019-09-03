#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

#include "Game.h"
#include "GAnchorSprite.h"
class GGamePlayfield;

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
  TBool IsWall(TFloat aX, TFloat aY);
  TBool IsFloor(TFloat aX, TFloat aY);
  TBool IsLedge(TFloat aX, TFloat aY);
  
  TBool CanWalk(TRect& aRect);
  
protected:
  void NewState(TUint16 aState, DIRECTION aDirection);
  TBool MaybeHit();
  TBool MaybeWalk();
  TBool MaybeSword();
  TBool MaybeFall();
  
protected:
  TBool IdleState();
  TBool WalkState();
  TBool SwordState();
  TBool FallState();
  TBool HitState();
  
protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  GAnchorSprite *mSprite;
  TUint16 mState;
  TUint16 mStep;
};


#endif //MODITE_GPLAYERPROCESS_H
