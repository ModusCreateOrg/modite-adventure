#include "GEnemyArrowProcess.h"
#include "GEnemyProjectileProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat PROJECTILE_VELOCITY = 3.5;
const TInt PROJECTILE_DAMAGE = 55;
const TInt16 PROJECTILE_SPEED = 4;

static void get_explode_animation(TInt16 *buf, TInt16 aImageNumber, TUint16 aSlot, TUint32 flags) {
#ifdef DEBUGME
  printf("USING ANIMATION %d\n", aImageNumber);
#endif
  TInt16 opcode;

  if (flags & SFLAG_FLIP) {
    if (flags & SFLAG_FLOP) {
      opcode = AFLIPFLOPI;
    } else {
      opcode = AFLIPI;
    }
  } else if (flags & SFLAG_FLOP) {
    opcode = AFLOPI;
  } else {
    opcode = ASTEPI;
  }

  buf[0] = ABITMAPI;
  buf[1] = aSlot;
  buf[2] = opcode;
  buf[3] = PROJECTILE_SPEED;
  buf[4] = aImageNumber + 4;
  buf[5] = opcode;
  buf[6] = PROJECTILE_SPEED;
  buf[7] = aImageNumber + 8;
  buf[8] = opcode;
  buf[9] = PROJECTILE_SPEED;
  buf[10] = aImageNumber + 12;
  buf[11] = AEND;
}

static const SpriteData SPRITE_DATA[] = {
  {
    .x = 0,
    .y = 16,
    .cx = 20,
    .cy = -12,
    .imageNumber = 0,
    .flags = SFLAG_RIGHT,
    .parry = {
      .x = 0,
      .y = 0,
    }
  },
  {
    .x = 0,
    .y = -32,
    .cx = 8,
    .cy = 64,
    .imageNumber = 0,
    .flags = SFLAG_FLOP,
    .parry = {
      .x = 0,
      .y = -64,
    }
  },
  {
    .x = 0,
    .y = 0,
    .cx = 8,
    .cy = -24,
    .imageNumber = 0,
    .flags = 0,
    .parry = {
      .x = 0,
      .y = 64,
    }
  },
  {
    .x = 0,
    .y = 16,
    .cx = -2,
    .cy = -12,
    .imageNumber = 0,
    .flags = SFLAG_LEFT,
    .parry = {
      .x = 0,
      .y = 0,
    }
  },
  {
    .x = 0,
    .y = -48,
    .cx = 18,
    .cy = 64,
    .imageNumber = 3,
    .flags = SFLAG_FLOP,
    .parry = {
      .x = 16,
      .y = -68,
    }
  },
  {
    .x = 0,
    .y = -48,
    .cx = 18,
    .cy = 64,
    .imageNumber = 2,
    .flags = SFLAG_FLOP,
    .parry = {
      .x = 16,
      .y = -60,
    }
  },
  {
    .x = 0,
    .y = -48,
    .cx = 12,
    .cy = 64,
    .imageNumber = 1,
    .flags = SFLAG_FLOP,
    .parry = {
      .x = 0,
      .y = -70,
    }
  },
  {
    .x = 0,
    .y = 16,
    .cx = 18,
    .cy = -24,
    .imageNumber = 3,
    .flags = 0,
    .parry = {
      .x = 16,
      .y = 68,
    }
  },
  {
    .x = 0,
    .y = 16,
    .cx = 18,
    .cy = -24,
    .imageNumber = 2,
    .flags = 0,
    .parry = {
      .x = 16,
      .y = 60,
    }
  },
  {
    .x = 0,
    .y = 16,
    .cx = 12,
    .cy = -24,
    .imageNumber = 1,
    .flags = 0,
    .parry = {
      .x = 0,
      .y = 70,
    }
  },
  {
    .x = -16,
    .y = -48,
    .cx = 4,
    .cy = 64,
    .imageNumber = 1,
    .flags = SFLAG_FLIP | SFLAG_FLOP,
    .parry = {
      .x = 0,
      .y = -68,
    }
  },
  {
    .x = -16,
    .y = -48,
    .cx = -2,
    .cy = 64,
    .imageNumber = 2,
    .flags = SFLAG_FLIP | SFLAG_FLOP,
    .parry = {
      .x = -16,
      .y = -60,
    }
  },
  {
    .x = -16,
    .y = -48,
    .cx = -2,
    .cy = 64,
    .imageNumber = 3,
    .flags = SFLAG_FLIP | SFLAG_FLOP,
    .parry = {
      .x = -16,
      .y = -70,
    }
  },
  {
    .x = -16,
    .y = 16,
    .cx = 4,
    .cy = -24,
    .imageNumber = 1,
    .flags = SFLAG_FLIP,
    .parry = {
      .x = 0,
      .y = 68,
    }
  },
  {
    .x = -16,
    .y = 16,
    .cx = -2,
    .cy = -24,
    .imageNumber = 2,
    .flags = SFLAG_FLIP,
    .parry = {
      .x = -16,
      .y = 60,
    }
  },
  {
    .x = -16,
    .y = 16,
    .cx = -2,
    .cy = -24,
    .imageNumber = 3,
    .flags = SFLAG_FLIP,
    .parry = {
      .x = -16,
      .y = 70,
    }
  }
};

// top, bottom, left, right and angles in steps of 22.5 degrees
static const TFloat ARROW_ANGLES[16] = {
  0,
  1.570796,
  -1.570796,
  M_PI,

  0.3926991,
  0.7853982,
  1.1780973,

  -0.3926991,
  -0.7853982,
  -1.1780973,

  1.9634954,
  2.3561944,
  2.7488935,

  -1.9634954,
  -2.3561944,
  -2.7488935,
};


void GEnemyArrowProcess::ConfigSprite(TFloat aAngle, TBool aParry) {
  TUint8 angleIndex = 0;
  TFloat angle = 0;
  TFloat deltaMin = 99.;

  // Find the closest angle to the player
  for (TUint8 i = 0; i < 16; i++) {
    const TFloat delta = ABS(aAngle - ARROW_ANGLES[i]);
    if (delta < deltaMin) {
      deltaMin = delta;
      angle = ARROW_ANGLES[i];
      angleIndex = i;
    }
  }

#ifdef DEBUGME
  printf("ANGLE INDEX %d\n", angleIndex);
#endif

  mAngle = angle;

  const SpriteData spriteData = SPRITE_DATA[angleIndex];
  mSprite->mImageNumber = spriteData.imageNumber;
  mSprite->cx = spriteData.cx;
  mSprite->cy = spriteData.cy;
  mSprite->SetFlags(spriteData.flags);
  if (aParry) {
    mSprite->x += spriteData.parry.x;
    mSprite->y += spriteData.parry.y;
  } else {
    mSprite->x += spriteData.x;
    mSprite->y += spriteData.y;
  }
}

GEnemyArrowProcess::GEnemyArrowProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg)
  : GEnemyProjectileProcess(aGameState, aX, aY, aSlot, aImg) {
  mSprite->w = 8;
  mSprite->h = 8;
  mSprite->mAttackStrength = PROJECTILE_DAMAGE;
  ConfigSprite(aAngle);

  // Angles are in radians
  mSprite->vx = COS(mAngle) * PROJECTILE_VELOCITY;
  mSprite->vy = SIN(mAngle) * PROJECTILE_VELOCITY;
#ifdef DEBUGME
  printf("ENEMY PROJECTILE at %f,%f with velocity of %f %f\n", mSprite->x, mSprite->y, mSprite->vx, mSprite->vy);
#endif
}

void GEnemyArrowProcess::Explode() {
  get_explode_animation(mExplodeAnimation, mSprite->mImageNumber, mSprite->mBitmapSlot, mSprite->flags);
  mSprite->StartAnimation(mExplodeAnimation);
}

void GEnemyArrowProcess::Parry() {
  gSoundPlayer.TriggerSfx(SFX_PLAYER_PARRY_PROJECTILE_WAV, 2);
  ConfigSprite(mAngle > 0 ? mAngle - M_PI : mAngle + M_PI, ETrue);
  mSprite->vx *= -1;
  mSprite->vy *= -1;
}



