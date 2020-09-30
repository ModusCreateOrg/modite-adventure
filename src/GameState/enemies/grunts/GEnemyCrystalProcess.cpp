#include "GPlayer.h"
#include "GEnemyCrystalProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_DAMAGE = 55;
const TInt16 PROJECTILE_SPEED = 4;

const TFloat PARRY_DEVIATION_BASE = 90 * M_PI / 180;
const TFloat PARRY_ACCURACY_BONUS_SWORD = 30 * M_PI / 180;
const TFloat PARRY_ACCURACY_BONUS_GLOVES = 45 * M_PI / 180;

static ANIMSCRIPT shootAnimation[] = {
  ABITMAP(PROJECTILE_CRYSTAL_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, 0),
  ASTEP(PROJECTILE_SPEED, 1),
  ASTEP(PROJECTILE_SPEED, 2),
  ASTEP(PROJECTILE_SPEED, 3),
  ALOOP,
};

static ANIMSCRIPT explodeAnimation[] = {
  ABITMAP(PROJECTILE_CRYSTAL_SLOT),
  ASTEP(PROJECTILE_SPEED, 4),
  ASTEP(PROJECTILE_SPEED, 5),
  ASTEP(PROJECTILE_SPEED, 6),
  ASTEP(PROJECTILE_SPEED, 7),
  AEND,
};

GEnemyCrystalProcess::GEnemyCrystalProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg)
  : GEnemyProjectileProcess(aGameState, aX, aY, aSlot, aImg) {
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(PROJECTILE_CRYSTAL_BMP_SPRITES);

  mAngle = aAngle;
  mSprite->w = 16;
  mSprite->h = 16;
  mSprite->cx = 0;
  mSprite->cy -= 8;
  mSprite->mAttackStrength = PROJECTILE_DAMAGE;

  // Angles are in radians
  mSprite->vx = COS(mAngle) * PROJECTILE_VELOCITY;
  mSprite->vy = SIN(mAngle) * PROJECTILE_VELOCITY;

  mSprite->StartAnimation(shootAnimation);
#ifdef DEBUGME
  printf("ENEMY PROJECTILE at %f,%f with velocity of %f %f\n", mSprite->x, mSprite->y, mSprite->vx, mSprite->vy);
#endif
}

void GEnemyCrystalProcess::Explode() {
  gSoundPlayer.TriggerSfx(SFX_TOWER_PROJECTILE_DEATH_WAV, 2);
  mSprite->StartAnimation(explodeAnimation);
}

void GEnemyCrystalProcess::Parry() {
  gSoundPlayer.TriggerSfx(SFX_PLAYER_PARRY_PROJECTILE_WAV, 5);
  if (GPlayer::mTargeted) {
    TPoint target = GPlayer::mTargeted->Center(), center = mSprite->Center();
    mAngle = ATAN2(target.y - center.y, target.x - center.x);
  } else {
    switch (GPlayer::mSprite->mDirection) {
      case DIRECTION_UP:
        mAngle = 3 * M_PI_2;
        break;
      case DIRECTION_DOWN:
        mAngle = M_PI_2;
        break;
      case DIRECTION_LEFT:
        mAngle = M_PI;
        break;
      case DIRECTION_RIGHT:
      default:
        mAngle = 0;
    }
  }
  // random deviation depending on equipped items
  TFloat deviationAmount = PARRY_DEVIATION_BASE;
  if (GPlayer::mEquipped.mGloves) {
    deviationAmount = MAX(0, deviationAmount - PARRY_ACCURACY_BONUS_GLOVES);
  }
  if (GPlayer::mEquipped.mSword) {
    deviationAmount = MAX(0, deviationAmount - PARRY_ACCURACY_BONUS_SWORD);
  }
  mAngle += (RandomFloat() - 0.5) * deviationAmount;
  mSprite->vx = COS(mAngle) * PROJECTILE_VELOCITY;
  mSprite->vy = SIN(mAngle) * PROJECTILE_VELOCITY;
}