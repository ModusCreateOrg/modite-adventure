#include <GameState/status/GStatProcess.h>
#include "GPlayer.h"
#include "Items.h"

TUint32 GPlayer::mLevel;
TUint32 GPlayer::mNextLevel;
TUint32 GPlayer::mExperience;
TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;

TInt GPlayer::mHitStrength;
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
  v = mEquipped.mAmulet ? mEquipped.mAmulet->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mRing ? mEquipped.mRing->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mGloves ? mEquipped.mGloves->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mBoots ? mEquipped.mBoots->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mWeapon ? mEquipped.mWeapon->mItemNumber : 0;
  stream.Write(&v, sizeof(v));
  v = mEquipped.mSpellbook ? mEquipped.mSpellbook->mItemNumber : 0;
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
  mEquipped.mAmulet = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mRing = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mGloves = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mBoots = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mWeapon = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mSpellbook = mInventoryList.FindItem(v);
}

void GPlayer::Dump() {
  printf("GPlayer\n");
  printf("%-32.32s: %d,%d/%d\n", "mLevel,mNextLevel, mExperience", mLevel, mNextLevel, mExperience);
  printf("%-32.32s: %d,%d/%d\n", "mHitPoints, mMaxHitPoints, mHitStrength", mHitPoints, mMaxHitPoints, mHitStrength);
  printf("%-32.32s: %d,%d\n", "mHealthPotion, mManaPotion", mHealthPotion, mManaPotion);
  mInventoryList.Dump();
}

TUint16 GPlayer::GetSpellSlot() {
  switch (GPlayer::mEquipped.mSpellbook->mItemNumber) {
    case ITEM_BLUE_SPELLBOOK:
      return SPELL_WATER_SLOT;
    case ITEM_RED_SPELLBOOK:
      return SPELL_FIRE_SLOT;
    case ITEM_GREEN_SPELLBOOK:
      return SPELL_EARTH_SLOT;
    case ITEM_YELLOW_SPELLBOOK:
      return SPELL_ELECTRICITY_SLOT;
    default:
      Panic("Invalid spell");
  }
  return 0;
}

TBool GPlayer::MaybeDamage(GAnchorSprite *aSprite, TBool aIsSpell) {
  if (!aSprite->mInvulnerable) {
    TInt hitAmount = mHitStrength;
    ELEMENT hitElement = ELEMENT_NONE;
    if (aIsSpell) {
      if (mEquipped.mSpellbook) {
        switch (mEquipped.mSpellbook->mItemNumber) {
          case ITEM_BLUE_SPELLBOOK:
            hitElement = ELEMENT_WATER;
            break;
          case ITEM_RED_SPELLBOOK:
            hitElement = ELEMENT_FIRE;
            break;
          case ITEM_GREEN_SPELLBOOK:
            hitElement = ELEMENT_EARTH;
            break;
          case ITEM_YELLOW_SPELLBOOK:
            hitElement = ELEMENT_ENERGY;
            break;
          default:
            break;
        }
        if (hitElement && aSprite->mElement) {
          hitAmount *= SPELLBOOK_MATRIX[aSprite->mElement - 1][hitElement - 1];
        } else {
          hitAmount *= SPELL_HIT_BONUS;
        }
      }
    } else {
      if (mEquipped.mRing) {
        switch (mEquipped.mRing->mItemNumber) {
          case ITEM_BLUE_RING:
            hitElement = ELEMENT_WATER;
            break;
          case ITEM_RED_RING:
            hitElement = ELEMENT_FIRE;
            break;
          case ITEM_GREEN_RING:
            hitElement = ELEMENT_EARTH;
            break;
          case ITEM_YELLOW_RING:
            hitElement = ELEMENT_ENERGY;
            break;
          default:
            break;
        }
        if (hitElement && aSprite->mElement) {
          hitAmount *= RING_MATRIX[aSprite->mElement - 1][hitElement - 1];
        } else {
          hitAmount *= RING_HIT_BONUS;
        }
      }
    }
    // Random +/- 20% damage modifier
    hitAmount = (hitAmount * Random(80, 120)) / 100;
    aSprite->mHitPoints -= hitAmount;
    auto *p = new GStatProcess(aSprite->x + 68, aSprite->y + 32, "%d", hitAmount);
    p->SetMessageType(STAT_ENEMY_HIT);
    aSprite->mGameState->AddProcess(p);
    gSoundPlayer.SfxEnemyTakeDamage();

    return ETrue;
  }
  return EFalse;
}
