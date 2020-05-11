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
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GEnemyProcess::SetStatMultipliers(TFloat aModHitPoints, TFloat aModStrength, TFloat aModExperience) {
  mHitPoints = mMaxHitPoints = round(aModHitPoints * BASE_HIT_POINTS);
  mExperienceYield = round(aModExperience * BASE_EXPERIENCE);
  if (mSprite) {
    mSprite->mAttackStrength = round(aModStrength * BASE_STRENGTH);
  }
}

TBool GEnemyProcess::MaybeDamage(TBool aIsSpell) {
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

TBool GEnemyProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyProcess::RunAfter() {
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
  return ETrue;
}
