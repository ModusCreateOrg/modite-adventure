#include "GOrcProcess.h"
#include "GStatProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 120;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 1.5 / FACTOR;

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */

ANIMSCRIPT idleAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(40, IMG_ORC_IDLE),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ASTEP(40, IMG_ORC_IDLE + 2),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
  AEND,
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 20),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 20),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_DOWN + 0),
  AEND,
}; // 20 24 18

static ANIMSCRIPT attackDoubleDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ADELTA(0, 20),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 20),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_DOWN + 0),
  AEND,
}; // 20 24 18

static ANIMSCRIPT attackRoundDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  AFILL(-1),
  AEND,
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-4, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(0, 0), //
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-4, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackDoubleLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-4, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(0, 0), //
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ADELTA(0, 0), //
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(-4, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackRoundLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND,
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */

static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(-4, 0), //
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackDoubleRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(-4, 0), //
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ADELTA(-4, 0), //
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ASIZE(0, 4, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackRoundRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND,
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */

static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 0),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP + 0),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT attackDoubleUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP + 0),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED * 6, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT attackRoundUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(-4, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12, 4, 56, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(14, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(14, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-14, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 16),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-14, 2),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-14, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),

  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,4,32,32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AFILL(-1),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  AEND,
};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GOrcProcess::GOrcProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GEnemyProcess(aGameState, aIp, ORC_SLOT, aParams, VELOCITY, ATTR_ORC) {
  mSprite->Name("ENEMY ORC");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_ORC_BMP_SPRITES);
  mSprite->SetStatMultipliers(4.0, 3.0, 4.0);
  mRangeX = 24;
  mRangeY = 16;
//  mSprite->mHitPoints = 1;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GOrcProcess::~GOrcProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GOrcProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(idleUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(idleDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(idleLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(idleRightAnimation);
      break;
    default:
      Panic("GIdleProcess no idle direction\n");
      break;
  }
}

void GOrcProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      mSprite->vy = -VELOCITY;
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
    default:
      Panic("GOrcProcess no walk direction\n");
      break;
  }
}

void GOrcProcess::Attack(DIRECTION aDirection) {
  TInt attackType = Random() & TUint8(3);

  // 25% chance for either round or double attack
  if (attackType < 2) {
    switch (mSprite->mDirection) {
      case DIRECTION_UP:
        mSprite->StartAnimation(attackUpAnimation);
        break;
      case DIRECTION_DOWN:
        mSprite->StartAnimation(attackDownAnimation);
        break;
      case DIRECTION_LEFT:
        mSprite->StartAnimation(attackLeftAnimation);
        break;
      case DIRECTION_RIGHT:
        mSprite->StartAnimation(attackRightAnimation);
        break;
      default:
        Panic("GOrcProcess no attack direction\n");
        break;
    }
  } else if (attackType == 2) {
    switch (mSprite->mDirection) {
      case DIRECTION_UP:
        mSprite->StartAnimation(attackRoundUpAnimation);
        break;
      case DIRECTION_DOWN:
        mSprite->StartAnimation(attackRoundDownAnimation);
        break;
      case DIRECTION_LEFT:
        mSprite->StartAnimation(attackRoundLeftAnimation);
        break;
      case DIRECTION_RIGHT:
        mSprite->StartAnimation(attackRoundRightAnimation);
        break;
      default:
        Panic("GOrcProcess no attack direction\n");
        break;
    }
  } else {
    switch (mSprite->mDirection) {
      case DIRECTION_UP:
        mSprite->StartAnimation(attackDoubleUpAnimation);
        break;
      case DIRECTION_DOWN:
        mSprite->StartAnimation(attackDoubleDownAnimation);
        break;
      case DIRECTION_LEFT:
        mSprite->StartAnimation(attackDoubleLeftAnimation);
        break;
      case DIRECTION_RIGHT:
        mSprite->StartAnimation(attackDoubleRightAnimation);
        break;
      default:
        Panic("GOrcProcess no attack direction\n");
        break;
    }
  }
}

void GOrcProcess::Hit(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(hitUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(hitDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(hitLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(hitRightAnimation);
      break;
    case DIRECTION_SPELL:
      mSprite->StartAnimation(hitSpellAnimation);
      break;
  }
}

void GOrcProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}
