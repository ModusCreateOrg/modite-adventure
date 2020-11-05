#include "GFinalBossPillarSprite.h"

#define DEBUGME
#undef DEBUGME

const TInt16 PILLAR_SPEED = 4;

static ANIMSCRIPT persistAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT warningAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ADELTA(2, 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ADELTA(0, 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ADELTA(-2, 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ADELTA(0, 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  AEND,
};

static ANIMSCRIPT explodeAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 6),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 7),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 8),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 9),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 10),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 11),
  AEND,
};

// constructor
GFinalBossPillarSprite::GFinalBossPillarSprite(GGameState *aGameState, TFloat aX, TFloat aY, ELEMENT aElement)
  : GAnchorSprite(aGameState, 0, EARTH_FINAL_BOSS_PILLAR_SLOT, 0, STYPE_EBULLET) {
  mGameState = aGameState;
  mElement = aElement;
#ifdef DEBUGME
  printf("Final Boss Pillar %p element %d\n", this, aElement);
#endif
  x = aX;
  y = aY;
  w = 16;
  h = 8;
  cy = 4;
  cx = 0;
  SetFlags(SFLAG_RENDER_SHADOW | SFLAG_KNOCKBACK);
  SetCMask(STYPE_PLAYER);
  ResetShadow();
  vy = vx = 0;
  mAttackStrength = 55;
  switch (aElement) {
    case ELEMENT_FIRE:
#ifdef DEBUGME
      printf("%s FIRE PILLAR\n", Name());
#endif
      mBitmapSlot = FIRE_FINAL_BOSS_PILLAR_SLOT;
      break;
    case ELEMENT_EARTH:
#ifdef DEBUGME
      printf("%s EARTH PILLAR\n", Name());
#endif
      mBitmapSlot = EARTH_FINAL_BOSS_PILLAR_SLOT;
      break;
    case ELEMENT_WATER:
#ifdef DEBUGME
      printf("%s WATER PILLAR\n", Name());
#endif
      mBitmapSlot = WATER_FINAL_BOSS_PILLAR_SLOT;
      break;
    case ELEMENT_ENERGY:
#ifdef DEBUGME
      printf("%s ENERGY PILLAR\n", Name());
#endif
      mBitmapSlot = ENERGY_FINAL_BOSS_PILLAR_SLOT;
      break;
    default:
      Panic("%s invalid element %d\n", Name(), mElement);
  }
  mExploding = EFalse;
}

void GFinalBossPillarSprite::Persist() {
  StartAnimation(persistAnimation);
}

void GFinalBossPillarSprite::Warn() {
  StartAnimation(warningAnimation);
}

void GFinalBossPillarSprite::Explode() {
  if (!mExploding) {
    StartAnimation(explodeAnimation);
    mExploding = ETrue;
  }
}
