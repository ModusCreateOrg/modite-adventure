/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

class GPlayerProcess;

#include <BMemoryStream.h>
#include <BResourceManager.h>
#include "GGameState.h"
#include "GPlayer.h"
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
  GInventoryItem *mAmulet,
    *mRing,
    *mGloves,
    *mBoots,
    *mWeapon,
    *mSpellbook;
};

struct GPlayer {
  static void Init() {
    printf("Init GPlayer\n");
    mLevel = 1;
    mNextLevel = 100;
    mExperience = 0;
    mMaxHitPoints = DEFAULT_PLAYER_HITPOINTS;
    mHitPoints = mMaxHitPoints;
    mHitStrength = 35;
    mHealthPotion = mManaPotion = 100;
    //
    mEquipped.mAmulet = ENull;
    mEquipped.mRing = ENull;
    mEquipped.mGloves = ENull;
    mEquipped.mBoots = ENull;
    mEquipped.mWeapon = ENull;
    mEquipped.mSpellbook = ENull;

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
      mHitStrength += 7;
    }
  }

  static void AddHitPoints(TInt aMoreHitpoints) {
    auto *p = new GStatProcess(mSprite->x + 72, mSprite->y + 32, "%d", MIN(aMoreHitpoints, mMaxHitPoints - mHitPoints));
    p->SetMessageType(STAT_HEAL);
    mSprite->mGameState->AddProcess(p);
    mHitPoints += aMoreHitpoints;
    if (mHitPoints > mMaxHitPoints) {
      mHitPoints = mMaxHitPoints;
    }
  }

  static TBool MaybeDamage(GAnchorSprite *aSprite, TBool aIsSpell);

  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);
  static TUint16 GetSpellSlot();

  static TUint32 mLevel;
  static TUint32 mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints;
  static TInt32 mHitStrength;
  static TInt32 mHealthPotion, mManaPotion; // 100, 75, 50, 25, 0 are possible values
  static GInventoryList mInventoryList;
  static GPlayerProcess *mProcess;
  static GAnchorSprite *mSprite;
  static GGameState *mGameState;
  static GEquipped mEquipped;
  static TBool mGameOver;
  static GAnchorSprite *mActiveBoss;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
