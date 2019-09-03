#ifndef MODITE_GENEMYPROCESS_H
#define MODITE_GENEMYPROCESS_H

#include <BProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

const TFloat SEEK_Y = COLLISION_DELTA_Y;  // seek to player Y within this many pixels
const TFloat SEEK_X = 32; // seek to player X within this many pixels

enum {
  IDLE_STATE,
  WALK_STATE,
  ATTACK_STATE,
  HIT_STATE,
  DEATH_STATE,
};

static const char  *stateMessages[] = {
  "IDLE STATE",
  "WALK STATE",
  "ATTACK STATE",
  "HIT STATE",
  "DEATH STATE",
};

class GEnemyProcess : public BProcess {
public:
  GEnemyProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TUint16 aSlot);

  virtual ~GEnemyProcess();

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  GAnchorSprite *mSprite;
  TUint16 mState;
  TUint16 mDirection;
  TUint16 mStep;
  TInt16 mAttackTimer;

  GAnchorSprite *mPlayerSprite;

protected:
  // test if a wall in the specified direction from sprite's current location
  TBool IsWall(TInt aDirection);

protected:
  virtual void NewState(TUint16 aState, DIRECTION aDirection) = 0;

  virtual TBool IdleState() = 0;

  virtual TBool WalkState() = 0;

  virtual TBool AttackState() = 0;

  virtual TBool HitState() = 0;

  virtual TBool DeathState() = 0;
};


#endif //MODITE_GENEMYPROCESS_H
