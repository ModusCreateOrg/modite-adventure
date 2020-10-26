#include "GFinalBossPillarSprite.h"

#define DEBUGME
#undef DEBUGME

const TInt16 PILLAR_SPEED = 4;

static ANIMSCRIPT pillarFireAnimation[] = {
  ABITMAP(FIRE_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};
static ANIMSCRIPT pillarEarthAnimation[] = {
  ABITMAP(EARTH_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT pillarWaterAnimation[] = {
  ABITMAP(WATER_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT pillarEnergyAnimation[] = {
  ABITMAP(ENERGY_FINAL_BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_FINAL_BOSS_PILLAR + 3),
  ALOOP,
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
GFinalBossPillarSprite::GFinalBossPillarSprite(GGameState *aGameState, TFloat aX, TFloat aY, TInt16 aSlot)
  : GAnchorSprite(aGameState, 0, aSlot, 0, STYPE_EBULLET) {
  mGameState = aGameState;
#ifdef DEBUGME
  // can't use mSprite->Name() since sprite hasn't been spawned yet
  printf("Final Boss Piller %p slot %d (earth %d)\n", this, aSlot, EARTH_FINAL_BOSS_PILLAR_SLOT);
#endif
  x = aX;
  y = aY;
  w = 16;
  h = 8;
  cy = 4;
  cx = 0;
  SetFlags(SFLAG_RENDER_SHADOW);
  ResetShadow();
  vy = vx = 0;
  mAttackStrength = 55;
  switch (aSlot) {
    case FIRE_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s FIRE PILLAR\n", mSprite->Name());
#endif
      StartAnimation(pillarFireAnimation);
      break;
    case EARTH_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s EARTH PILLAR\n", mSprite->Name());
#endif
      StartAnimation(pillarEarthAnimation);
      break;
    case WATER_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s WATER PILLAR\n", mSprite->Name());
#endif
      StartAnimation(pillarWaterAnimation);
      break;
    case ENERGY_FINAL_BOSS_PILLAR_SLOT:
#ifdef DEBUGME
      printf("%s ENERGY PILLAR\n", mSprite->Name());
#endif
      StartAnimation(pillarEnergyAnimation);
      break;
    default:
      Panic("%s invalid slot %d\n", Name(), aSlot);
  }
  mExploding = EFalse;
}

void GFinalBossPillarSprite::Explode() {
  if (!mExploding) {
    StartAnimation(explodeAnimation);
    mExploding = ETrue;
  }
}
