#include "GLivingProcess.h"

GLivingProcess::GLivingProcess(GGameState *aGameState, TUint16 aAttribute) : GProcess(aAttribute), mGameState(aGameState) {
  mInvulnerable = EFalse;
  mBlinkTimer = 0;
  mStep = 0;
  mState = 0;

  mHitPoints = mMaxHitPoints = BASE_HIT_POINTS;
  mExperienceYield = BASE_EXPERIENCE;
}

GLivingProcess::~GLivingProcess() = default;

void GLivingProcess::SetStatMultipliers(TFloat aModHitPoints, TFloat aModStrength, TFloat aModExperience) {
  mHitPoints = mMaxHitPoints = round(aModHitPoints * BASE_HIT_POINTS);
  mExperienceYield = round(aModExperience * BASE_EXPERIENCE);
  if (mSprite) {
    mSprite->mAttackStrength = round(aModStrength * BASE_STRENGTH);
  }
}

TBool GLivingProcess::MaybeDamage(TBool aIsSpell) {
  if (!mSprite || mInvulnerable) {
    return EFalse;
  }
  TInt attackAmount;
  if (aIsSpell) {
    attackAmount = GPlayer::mAttackStrength;
    if (GPlayer::mEquipped.mSpellBookElement && mSprite->mElement) {
      attackAmount *= SPELLBOOK_MATRIX[mSprite->mElement - 1][GPlayer::mEquipped.mSpellBookElement - 1];
    } else {
      attackAmount *= SPELL_ATTACK_BONUS;
    }
  } else {
    attackAmount = mSprite->mCollided->mAttackStrength;

    // Be able to attack a mid-boss with a like ring
    if (GPlayer::mEquipped.mRingElement && mSprite->mElement) {
      attackAmount *= RING_MATRIX[mSprite->mElement - 1][GPlayer::mEquipped.mRingElement - 1];
    }
    else {
      attackAmount *= RING_HIT_BONUS;
    }
  }

  // Random +/- 20% damage modifier
  attackAmount = (attackAmount * Random(80, 120)) / 100;
  mHitPoints -= attackAmount;
  auto *p = new GStatProcess(mSprite->x + 68, mSprite->y + 32, "%d", attackAmount);
  p->SetMessageType(STAT_ENEMY_HIT);
  mSprite->mGameState->AddProcess(p);
  gSoundPlayer.TriggerSfx(SFX_ENEMY_TAKE_DAMAGE_WAV);

  return ETrue;
}

TBool GLivingProcess::RunBefore() {
  return ETrue;
}

TBool GLivingProcess::RunAfter() {
  if (mBlinkTimer > 1) {
    mBlinkTimer--;
    if ((mBlinkTimer & 1u) == 0) {
      BlinkOn();
    } else {
      BlinkOff();
    }
  } else if (mBlinkTimer == 1) {
    mBlinkTimer--;
    BlinkOff();
    mInvulnerable = EFalse;
  }
  return ETrue;
}
