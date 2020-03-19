/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

class GPlayerProcess;
class GLivingProcess;

#include <BMemoryStream.h>
#include <BResourceManager.h>
#include "GGame.h"
#include "GGameState.h"
#include "GPlayerSprite.h"
#include "GAnchorSprite.h"
#include "GResources.h"
#include "inventory/GInventoryList.h"
#include "GStatProcess.h"

const TInt DEFAULT_PLAYER_HITPOINTS = 200;

const TFloat SPELLBOOK_MATRIX[4][4] = {
        // water,   fire,  earth, energy
        { 1.0,    1.0,    1.0,    1.0 }, // water
        { 1.5,    1.0,    1.5,    1.5 }, // fire
        { 1.0,    1.5,    1.0,    1.5 }, // earth
        { 1.5,    1.5,    1.5,    1.0 }, // energy
};

const TFloat RING_MATRIX[4][4] = {
        // water,   fire,  earth, energy
        { 1.0 ,   1.0 ,   1.0 ,   1.0  }, // water
        { 1.25,   1.0 ,   1.25,   1.25 }, // fire
        { 1.0 ,   1.25,   1.0 ,   1.25 }, // earth
        { 1.25,   1.25,   1.25,   1.0  }, // energy
};

const TFloat AMULET_MATRIX[4][4] = {
        // water,   fire,  earth, energy
        { 0.25,   2.0 ,   2.0 ,   2.0  }, // water
        { 1.0 ,   0.25,   1.0 ,   1.0  }, // fire
        { 1.0 ,   1.0 ,   0.25,   1.0  }, // earth
        { 2.0 ,   1.0 ,   1.0 ,   0.25 }, // energy
};

struct GEquipped {
  ELEMENT mAmuletElement, mRingElement, mSpellBookElement;
  GInventoryItem
    *mGloves,
    *mBoots,
    *mSword;

  void Init() {
    mAmuletElement = ELEMENT_NONE;
    mRingElement = ELEMENT_NONE;
    mSpellBookElement = ELEMENT_NONE;

    mGloves = ENull;
    mBoots = ENull;
    mSword = ENull;
  }
};

struct GPlayer {
  static void Init() {
    printf("Init GPlayer\n");
    mInventoryList.FullReset();
    mLevel = 1;
    mNextLevel = 100;
    mExperience = 0;
    mMaxHitPoints = DEFAULT_PLAYER_HITPOINTS;
    mHitPoints = mMaxHitPoints;
    mHitPointsHealed = 0;
    mAttackStrength = 35;
    mManaPotion = 100;

    //
    mEquipped.Init();

    mGameOver = EFalse;
    mActiveBoss = ENull;
  }

  static void AddExperience(TInt aExperience) {
    mExperience += aExperience;
    while (mExperience >= mNextLevel) {
      mLevel++;
      mExperience -= mNextLevel;
      mNextLevel += 100 + (mLevel - 1) * 50;
      mMaxHitPoints += 40;
      mHitPoints = mMaxHitPoints;
      mAttackStrength += 7;
    }
  }

  static void AddHitPoints(TInt aMoreHitpoints, TBool aShowStat = ETrue) {
    if (aShowStat) {
      auto *p = new GStatProcess(gGameEngine, mSprite->x + 72, mSprite->y + 32, "%d", MIN(aMoreHitpoints, mMaxHitPoints - mHitPoints + mHitPointsHealed));
      p->SetMessageType(STAT_HEAL);
      gGameEngine->AddProcess(p);
      mHitPointsHealed = 0;
    } else {
      mHitPointsHealed += aMoreHitpoints;
    }
    mHitPoints += aMoreHitpoints;
    if (mHitPoints > mMaxHitPoints) {
      mHitPoints = mMaxHitPoints;
    }
  }

  static TBool MaybeDamage(GLivingProcess *aProcess, TBool aIsSpell);

  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);
  static TUint16 GetSpellSlot();

  static TUint32 mLevel;
  static TUint32 mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints, mHitPointsHealed;
  static TInt32 mAttackStrength;
  static TInt32 mManaPotion; // 100, 75, 50, 25, 0 are possible values
  static GInventoryList mInventoryList;
  static GPlayerProcess *mProcess;
  static GPlayerSprite *mSprite;
  static GGameState *mGameState;
  static GEquipped mEquipped;
  static TBool mGameOver;
  static GAnchorSprite *mActiveBoss;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
