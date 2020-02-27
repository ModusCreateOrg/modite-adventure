#include <GameState/status/GStatProcess.h>
#include "GPlayer.h"
#include "Items.h"

TUint32 GPlayer::mLevel;
TUint32 GPlayer::mNextLevel;
TUint32 GPlayer::mExperience;
TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;

TInt GPlayer::mAttackStrength;
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
  stream.Write(&mManaPotion, sizeof(mManaPotion));
  mInventoryList.WriteToStream(stream);

  // Equipped
  stream.Write(&mEquipped.mAmuletElement, sizeof(mEquipped.mAmuletElement));
  stream.Write(&mEquipped.mRingElement, sizeof(mEquipped.mRingElement));
  stream.Write(&mEquipped.mSpellBookElement, sizeof(mEquipped.mSpellBookElement));
  TUint16 v;
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
  stream.Read(&mManaPotion, sizeof(mManaPotion));
  mInventoryList.ReadFromStream(stream);

  // Equipped
  stream.Read(&mEquipped.mAmuletElement, sizeof(mEquipped.mAmuletElement));
  stream.Read(&mEquipped.mRingElement, sizeof(mEquipped.mRingElement));
  stream.Read(&mEquipped.mSpellBookElement, sizeof(mEquipped.mSpellBookElement));
  TUint16 v;
  stream.Read(&v, sizeof(v));
  mEquipped.mGloves = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mBoots = mInventoryList.FindItem(v);
  stream.Read(&v, sizeof(v));
  mEquipped.mSword = mInventoryList.FindItem(v);
}

void GPlayer::Dump() {
  printf("GPlayer\n");
  printf("%-32.32s: %d,%d/%d\n", "mLevel,mNextLevel, mExperience", mLevel, mNextLevel, mExperience);
  printf("%-32.32s: %d,%d/%d\n", "mHitPoints, mMaxHitPoints, mAttackStrength", mHitPoints, mMaxHitPoints, mAttackStrength);
  printf("%-32.32s: %d\n", "mManaPotion", mManaPotion);
  mInventoryList.Dump();
}

TUint16 GPlayer::GetSpellSlot() {
  switch (GPlayer::mEquipped.mSpellBookElement) {
    case ELEMENT_WATER:
      return SPELL_WATER_SLOT;
    case ELEMENT_FIRE:
      return SPELL_FIRE_SLOT;
    case ELEMENT_EARTH:
      return SPELL_EARTH_SLOT;
    case ELEMENT_ENERGY:
      return SPELL_ELECTRICITY_SLOT;
    default:
      Panic("Invalid spell");
  }
  return 0;
}

TBool GPlayer::MaybeDamage(GAnchorSprite *aSprite, TBool aIsSpell) {
  if (!aSprite->mInvulnerable) {
    TInt attackAmount = mAttackStrength;

    if (aIsSpell) {
      if (mEquipped.mSpellBookElement && aSprite->mElement) {
        attackAmount *= SPELLBOOK_MATRIX[aSprite->mElement - 1][mEquipped.mSpellBookElement - 1];
      } else {
        attackAmount *= SPELL_ATTACK_BONUS;
      }
    }
    else {

      // Be able to attack a mid-boss with a like ring
      if (mEquipped.mRingElement && aSprite->mElement) {
        attackAmount *= RING_MATRIX[aSprite->mElement - 1][mEquipped.mRingElement - 1];
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
