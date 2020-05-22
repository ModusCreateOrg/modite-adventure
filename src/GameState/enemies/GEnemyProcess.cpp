#include "GEnemyProcess.h"

GEnemyProcess::GEnemyProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute)
    : GProcess(aAttribute), mGameState(aGameState), mPlayfield(aGameState->mGamePlayfield) {
  mSprite = new GEnemySprite(mGameState, ENEMY_PRIORITY, aSlot, 0, STYPE_ENEMY);
  mSprite->Name("ENEMY");
  mGameState->AddSprite(mSprite);

  mSprite->x = mStartX = aX;
  mSprite->y = mStartY = aY;
  mDirection = mSprite->mDirection = DIRECTION_DOWN;
  mInvulnerable = EFalse;
  mBlinkTimer = 0;
  mStep = 0;
  mState = 0;

  mHitPoints = mMaxHitPoints = BASE_HIT_POINTS;
  mExperienceYield = BASE_EXPERIENCE;
}

GEnemyProcess::~GEnemyProcess() {
  if (mHitPoints <= 0) {
    mGameState->AddProcess(new GStatProcess(STAT_EXPERIENCE, mSprite->Center(), "EXP +%d", mExperienceYield));
    GPlayer::AddExperience(mExperienceYield);
  }

  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GEnemyProcess::SetStatMultipliers(TFloat aModHitPoints, TFloat aModStrength, TFloat aModExperience) {
  mHitPoints = mMaxHitPoints = round(aModHitPoints * BASE_HIT_POINTS);
  mExperienceYield = round(aModExperience * BASE_EXPERIENCE);
  mSprite->mAttackStrength = round(aModStrength * BASE_STRENGTH);
}

TBool GEnemyProcess::BasicDamageCheck() {
  if (mInvulnerable || !mSprite->TestAndClearCType(STYPE_PBULLET)) {
    return EFalse;
  }

  TInt attackAmount = mSprite->mCollided->mAttackStrength;
  // Be able to attack a mid-boss with a like ring
  if (GPlayer::mEquipped.mRingElement) {
    if (mSprite->mElement) {
      attackAmount *= RING_MATRIX[mSprite->mElement - 1][GPlayer::mEquipped.mRingElement - 1];
    } else {
      attackAmount *= RING_HIT_BONUS;
    }
  }
  DoDamage(attackAmount);

  return ETrue;
}

TBool GEnemyProcess::SpellDamageCheck() {
  if (mInvulnerable || !mSprite->TestAndClearCType(STYPE_SPELL)) {
    return EFalse;
  }

  TInt attackAmount = GPlayer::mAttackStrength;
  if (GPlayer::mEquipped.mSpellBookElement && mSprite->mElement) {
    attackAmount *= SPELLBOOK_MATRIX[mSprite->mElement - 1][GPlayer::mEquipped.mSpellBookElement - 1];
  } else {
    attackAmount *= SPELL_ATTACK_BONUS;
  }
  DoDamage(attackAmount);

  return ETrue;
}

void GEnemyProcess::DoDamage(TInt aStrength) {
  // Random +/- 20% damage modifier
  aStrength = (aStrength * Random(80, 120)) / 100;
  mHitPoints -= aStrength;
  mGameState->AddProcess(new GStatProcess(STAT_ENEMY_HIT, mSprite->Center(), "%d", aStrength));
  gSoundPlayer.TriggerSfx(SFX_ENEMY_TAKE_DAMAGE_WAV);
  mInvulnerable = ETrue;
}

void GEnemyProcess::DoHeal(TInt aAmount) {
  // TODO: @jaygarcia SfxWizardHeal (or use existing SfxPlayerQuaffHealthPotion)
  TInt healAmount = MIN(aAmount, mMaxHitPoints - mHitPoints);
  mGameState->AddProcess(new GStatProcess(STAT_HEAL, mSprite->Center(), "%d", healAmount));
  mHitPoints = MIN(mHitPoints + healAmount, mMaxHitPoints);
}

void GEnemyProcess::UpdateBlink() {
  if (mBlinkTimer > 1) {
    mBlinkTimer--;
    if ((mBlinkTimer & 1u) == 0) {
      mSprite->mFill = COLOR_WHITE;
    } else {
      mSprite->mFill = -1;
    }
  } else if (mBlinkTimer == 1) {
    mBlinkTimer--;
    mSprite->mFill = -1;
    mInvulnerable = EFalse;
  }
}
