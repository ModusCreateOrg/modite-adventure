#include <GameState/status/GStatProcess.h>
#include "GPlayer.h"
#include "Items.h"

TUint32 GPlayer::mLevel;
TUint32 GPlayer::mNextLevel;
TUint32 GPlayer::mExperience;
TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;

TInt GPlayer::mAttackStrength;
TInt GPlayer::mHealthPotion;
TInt GPlayer::mManaPotion;

TBool GPlayer::mGameOver;

GInventoryList GPlayer::mInventoryList;
GPlayerProcess *GPlayer::mProcess;
GAnchorSprite *GPlayer::mSprite;
GGameState *GPlayer::mGameState;
GEquipped GPlayer::mEquipped;
GAnchorSprite *GPlayer::mActiveBoss;

void GPlayer::WriteToStream(BMemoryStream &stream) {
  stream.Write(&mLevel, sizeof(mLevel));
  stream.Write(&mNextLevel, sizeof(mNextLevel));
  stream.Write(&mExperience, sizeof(mExperience));
//  stream.Write(&mHitPoints, sizeof(mHitPoints));
  stream.Write(&mMaxHitPoints, sizeof(mMaxHitPoints));
  stream.Write(&mHealthPotion, sizeof(mHealthPotion));
  stream.Write(&mManaPotion, sizeof(mManaPotion));
  mInventoryList.WriteToStream(stream);

  // Equipped
  TUint16 v;
  v = mEquipped.mWaterAmulet ? mEquipped.mWaterAmulet->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mFireAmulet ? mEquipped.mFireAmulet->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mEarthAmulet ? mEquipped.mEarthAmulet->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mEnergyAmulet ? mEquipped.mEnergyAmulet->mItemNumber : 0;
  stream.Write(&v, sizeof(v));

  v = mEquipped.mWaterRing ? mEquipped.mWaterRing->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mFireRing ? mEquipped.mFireRing->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mEarthRing ? mEquipped.mEarthRing->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mEnergyRing ? mEquipped.mEnergyRing->mItemNumber : 0;
  stream.Write(&v, sizeof(v));


  v = mEquipped.mSpellBook ? mEquipped.mSpellBook->mItemNumber : 0;
  stream.Write(&v, sizeof(v));

  v = mEquipped.mGloves ? mEquipped.mGloves->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mBoots ? mEquipped.mBoots->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mSword ? mEquipped.mSword->mItemNumber : 0;
  stream.Write(&v, sizeof(v));


}

void GPlayer::ReadFromStream(BMemoryStream &stream) {
  stream.Read(&mLevel, sizeof(mLevel));
  stream.Read(&mNextLevel, sizeof(mNextLevel));
  stream.Read(&mExperience, sizeof(mExperience));
//  stream.Read(&mHitPoints, sizeof(mHitPoints));
  stream.Read(&mMaxHitPoints, sizeof(mMaxHitPoints));
  stream.Read(&mHealthPotion, sizeof(mHealthPotion));
  stream.Read(&mManaPotion, sizeof(mManaPotion));
  mInventoryList.ReadFromStream(stream);

  // Equipped
  TUint16 v;
  stream.Read(&v, sizeof(v));
  mEquipped.mWaterAmulet = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mFireAmulet = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mEarthAmulet = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mEnergyAmulet = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));

  mEquipped.mWaterRing = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mFireRing = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mEarthRing = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mEnergyRing = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));


  mEquipped.mGloves = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mBoots = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mSword = mInventoryList.FindItem(v);

  stream.Read(&v, sizeof(v));
  mEquipped.mSpellBook = mInventoryList.FindItem(v);
}

void GPlayer::Dump() {
  printf("GPlayer\n");
  printf("%-32.32s: %d,%d/%d\n", "mLevel,mNextLevel, mExperience", mLevel, mNextLevel, mExperience);
  printf("%-32.32s: %d,%d/%d\n", "mHitPoints, mMaxHitPoints, mAttackStrength", mHitPoints, mMaxHitPoints, mAttackStrength);
  printf("%-32.32s: %d,%d\n", "mHealthPotion, mManaPotion", mHealthPotion, mManaPotion);
  mInventoryList.Dump();
}

TUint16 GPlayer::GetSpellSlot() {
  switch (GPlayer::mEquipped.mSpellBook->mItemNumber) {
    case ITEM_WATER_SPELLBOOK:
      return SPELL_WATER_SLOT;
    case ITEM_FIRE_SPELLBOOK:
      return SPELL_FIRE_SLOT;
    case ITEM_EARTH_SPELLBOOK:
      return SPELL_EARTH_SLOT;
    case ITEM_ENERGY_SPELLBOOK:
      return SPELL_ELECTRICITY_SLOT;
    default:
      Panic("Invalid spell");
  }
  return 0;
}

TBool GPlayer::MaybeDamage(GAnchorSprite *aSprite, TBool aIsSpell) {
  if (!aSprite->mInvulnerable) {
    TInt attackAmount = mAttackStrength;

    ELEMENT attackElement = ELEMENT_NONE;
    if (aIsSpell) {
      if (mEquipped.mSpellBook) {
        switch (mEquipped.mSpellBook->mItemNumber) {
          case ITEM_WATER_SPELLBOOK:
            attackElement = ELEMENT_WATER;
            break;
          case ITEM_FIRE_SPELLBOOK:
            attackElement = ELEMENT_FIRE;
            break;
          case ITEM_EARTH_SPELLBOOK:
            attackElement = ELEMENT_EARTH;
            break;
          case ITEM_ENERGY_SPELLBOOK:
            attackElement = ELEMENT_ENERGY;
            break;
          default:
            break;
        }

        if (attackElement && aSprite->mElement) {
          attackAmount *= SPELLBOOK_MATRIX[aSprite->mElement - 1][attackElement - 1];
        } else {
          attackAmount *= SPELL_ATTACK_BONUS;
        }
      }
    }
    else {

      // Be able to attack a mid-boss with a like ring
      if (mEquipped.mWaterRing && aSprite->mElement == ELEMENT_WATER) {
        attackAmount *= RING_MATRIX[aSprite->mElement - 1][ELEMENT_WATER - 1];
      }
      else if (mEquipped.mEarthRing && aSprite->mElement == ELEMENT_EARTH) {
        attackAmount *= RING_MATRIX[aSprite->mElement - 1][ELEMENT_EARTH - 1];
      }
      else if (mEquipped.mFireRing && aSprite->mElement == ELEMENT_FIRE) {
        attackAmount *= RING_MATRIX[aSprite->mElement - 1][ELEMENT_FIRE - 1];
      }
      else if (mEquipped.mEnergyRing && aSprite->mElement == ELEMENT_ENERGY) {
        attackAmount *= RING_MATRIX[aSprite->mElement - 1][ELEMENT_ENERGY - 1];
      }
      else {
        attackAmount *= RING_HIT_BONUS;
      }

    }
    // Random +/- 20% damage modifier
    attackAmount = (attackAmount * Random(80, 120)) / 100;
    aSprite->mHitPoints -= attackAmount;
    auto *p = new GStatProcess(aSprite->x + 68, aSprite->y + 32, "%d", attackAmount);
    p->SetMessageType(STAT_ENEMY_HIT);
    aSprite->mGameState->AddProcess(p);
    gSoundPlayer.SfxEnemyTakeDamage();

    return ETrue;
  }
  return EFalse;
}
