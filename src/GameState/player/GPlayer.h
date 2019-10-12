/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

#include "Game.h"
#include "inventory/GItemProcess.h"
#include "GPlayerProcess.h"
#include "GPlayer.h"

/**
 * Control bits definitions
 */
const TUint16 CONTROL_JOYUP = JOYUP;
const TUint16 CONTROL_JOYDOWN = JOYDOWN;
const TUint16 CONTROL_JOYLEFT = JOYLEFT;
const TUint16 CONTROL_JOYRIGHT = JOYRIGHT;
const TUint16 CONTROL_FIRE = BUTTONA;

struct GInventoryItem : public BNode {
  GInventoryItem(TInt aItemNumber);

  TInt mItemNumber;
  TInt mImage;
  TInt mCount;

  void Render();
};

struct GInventoryList : public BList {
  GInventoryList();

  void AddItem(GInventoryItem *aItem) {
    AddHead(*aItem);
  }

  void RemoveItem(GInventoryItem *aItem) {
    aItem->Remove();
  }

  void DropItem(GInventoryItem *aItem);

  void PickupItem(TInt aItemNumber);

  GInventoryItem *First() {
    return (GInventoryItem *)BList::First();
  }

  TBool End(GInventoryItem *item) {
    return BList::End((BNode *)item);
  }

  GInventoryItem *Next(GInventoryItem *item) {
    return (GInventoryItem *)BList::Next((BNode *)item);
  }

  void Dump();
};

struct GPlayer {
  static void Init() {
    printf("Construct GPlayer\n");
    mLevel = 1;
    mNextLevel = 10;
    mExperience = 0;
    mMaxHitPoints = 10;
    mHitPoints = mMaxHitPoints;
    mStrength = 10;
    mDexterity = 10;
    mHitStrength = 1;
    mGold = 0;
  }

  static void AddExperience(TInt aExperience) {
    mExperience += aExperience;
    if (mExperience >= mNextLevel) {
      mLevel++;
      mExperience -= mNextLevel;
      mNextLevel += mLevel * 2;
      mMaxHitPoints += 2;
      mHitPoints = mMaxHitPoints;
      mHitStrength++;
    }
  }

  static TInt16 mLevel, mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints;
  static TInt16 mStrength, mDexterity;
  static TInt mHitStrength;
  static TInt mGold;
  static GInventoryList mInventoryList;
  static GPlayerProcess *mProcess;
  static GAnchorSprite *mSprite;
  static GGameState *mGameState;
};

#endif //MODITE_GPLAYER_H
