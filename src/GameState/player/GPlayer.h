/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

class GPlayerProcess;
class GBossProcess;

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
const TInt DEFAULT_PLAYER_MANA = 100;

const TFloat SPELLBOOK_MATRIX[4][4] = {
  // water, fire,   earth,  energy (spell book)
  { 0.1,    0.5,    0.5,    0.5 }, // water (enemy)
  { 2.0,    0.1,    1.5,    2.0 }, // fire
  { 0.5,    2.0,    0.1,    1.5 }, // earth
  { 1.5,    1.5,    2.0,    0.1 }, // energy
};

const TFloat RING_MATRIX[4][4] = {
  // water, fire,   earth,  energy
  { 1.0 ,   1.0 ,   1.0 ,   1.0  }, // water
  { 1.25,   1.0 ,   1.25,   1.25 }, // fire
  { 1.0 ,   1.25,   1.0 ,   1.25 }, // earth
  { 1.25,   1.25,   1.25,   1.0  }, // energy
};

const TFloat AMULET_MATRIX[4][4] = {
  // water, fire,   earth,  energy
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
    mInvulnerable = EFalse;
    mAttackStrength = 35;
    mMaxMana = DEFAULT_PLAYER_MANA;
    mManaPotion = mMaxMana;

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
      mMaxMana += 20;
      mHitPoints = mMaxHitPoints;
      mManaPotion = mMaxMana;
      mAttackStrength += 7;
    }
  }

  static void AddHitPoints(TInt aMoreHitpoints, const TPoint &aPoint = GPlayer::mSprite->Center()) {
    gGame->CurrentState()->AddProcess(new GStatProcess(STAT_HEAL, aPoint,
      "%d", MIN(aMoreHitpoints, mMaxHitPoints - mHitPoints)));

    mHitPoints += aMoreHitpoints;
    if (mHitPoints > mMaxHitPoints) {
      mHitPoints = mMaxHitPoints;
    }
  }

  static void AddMana(TInt aMoreMana, const TPoint &aPoint = GPlayer::mSprite->Center()) {
    gGame->CurrentState()->AddProcess(new GStatProcess(STAT_MANA, aPoint,
      "%d", MIN(aMoreMana, mMaxMana - mManaPotion)));

    mManaPotion += aMoreMana;
    if (mManaPotion > mMaxMana) {
      mManaPotion = mMaxMana;
    }
  }

  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);
  static TUint16 GetSpellSlot();

  static TUint32 mLevel;
  static TUint32 mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints;
  static TBool mInvulnerable;
  static TInt32 mAttackStrength;
  static TInt32 mManaPotion, mMaxMana;
  static GInventoryList mInventoryList;
  static GPlayerProcess *mProcess;
  static GPlayerSprite *mSprite;
  static GGameState *mGameState;
  static GEquipped mEquipped;
  static TBool mGameOver;
  static GBossProcess *mActiveBoss;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
