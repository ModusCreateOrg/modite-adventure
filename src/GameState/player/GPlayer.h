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

const TInt DEFAULT_PLAYER_HITPOINTS = 200;

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
    printf("Construct GPlayer\n");
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

  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);

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

  static void Dump();
};

#endif //MODITE_GPLAYER_H
