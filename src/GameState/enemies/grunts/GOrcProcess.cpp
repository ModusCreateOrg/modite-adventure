#include "GOrcProcess.h"
#include "GStatProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 50;

const TInt IDLE_SPEED = 3 * FACTOR;
const TInt TAUNT_SPEED = 3 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 1;

const TFloat VELOCITY = .65 * PLAYER_VELOCITY;

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


static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_ORC_IDLE + 0),
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
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ADELTA(0, 26),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 16),
  ASTEP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDoubleDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_ORC_ATTACK_DOWN + 0),
  ADELTA(0, 26),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ASIZE(0, 0, 32, 16),

  /** Second Round **/
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ADELTA(0, 26),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED , IMG_ORC_ATTACK_DOWN + 2),
  ASIZE(0, 0, 32, 16),

  /** Third Round **/
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ADELTA(0, 26),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 26),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 16),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ASTEP(ATTACK_SPEED * 3, IMG_ORC_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),

  AEND,
}; // 20 24 18

static ANIMSCRIPT attackRoundDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  /* DOWN */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED  * 4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 25),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 25),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 15),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  /* LEFT */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(-3, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-34, 4, 56, 32),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  /* UP */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  /* RIGHT */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  /* Down */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 25),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 25),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 15),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  AEND,
};
static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(DEATH_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  AEND,
};

static ANIMSCRIPT hitSpellDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
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
  ADELTA(-3, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-3, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-3, 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-3, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-29, 4, 51, 26),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  AFLIP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackDoubleLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-29, 4, 51, 26),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 16),

  /*** second round **/
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-29, 4, 51, 26),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 16),

  /*** Third round **/
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-29, 4, 51, 26),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  AFLIP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackRoundLeftAnimation[] = {
  /* LEFT */
  ABITMAP(ORC_SLOT),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-29, 4, 51, 26),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  /* UP */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

  /* RIGHT */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  /* DOWN */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 25),
  ASIZE(0, 24, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 25),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 15),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

  /* LEFT */
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-18, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-36, 4, 56, 30),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-3, 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ADELTA(1, 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ADELTA(1, 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ADELTA(-2, 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AEND,
};


static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(-3, 0),
  AFLIP(DEATH_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitSpellLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackDoubleRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 16),

  /** Second round **/
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),

  /** Third round **/
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(5, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 4, 52, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(5, 0),
  ASTEP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),

  AEND,
};

static ANIMSCRIPT attackRoundRightAnimation[] = {
    /* RIGHT */
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
    ADELTA(5, 0),
    ATYPE(STYPE_EBULLET),
    ASIZE(-10, 4, 52, 32),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

    /* DOWN */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
    ATYPE(STYPE_EBULLET),
    ADELTA(0, 25),
    ASIZE(0, 24, 32, 32),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
    ADELTA(0, 25),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
    ADELTA(0, 15),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

    /* LEFT */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(-3, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
    ADELTA(-18, 0),
    ATYPE(STYPE_EBULLET),
    ASIZE(-29, 4, 51, 26),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
    ADELTA(-18, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
    ADELTA(-18, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),

    /* UP */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 6),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
    ATYPE(STYPE_EBULLET),
    ADELTA(0, 10),
    ASIZE(0, 0, 32, 52),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

    /* RIGHT */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 52),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
    ADELTA(5, 0),
    ATYPE(STYPE_EBULLET),
    ASIZE(-10, 4, 52, 32),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitSpellRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackDoubleUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
  ADELTA(0, 10),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ASIZE(0, 0, 32, 16),

  /** Second Round **/
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ASIZE(0, 0, 32, 16),

  /** Third Round **/
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ADELTA(0, 10),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 32, 52),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ASTEP(ATTACK_SPEED * 5, IMG_ORC_ATTACK_UP + 3),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  AEND,
};

static ANIMSCRIPT attackRoundUpAnimation[] = {
    ABITMAP(ORC_SLOT),
    /* UP */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 6),
    ASTEP(ATTACK_SPEED * 4, IMG_ORC_ATTACK_UP + 0),
    ATYPE(STYPE_EBULLET),
    ADELTA(0, 10),
    ASIZE(0, 0, 32, 52),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

    /* RIGHT */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
    ADELTA(5, 0),
    ATYPE(STYPE_EBULLET),
    ASIZE(-10, 4, 52, 32),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
    ADELTA(5, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
    /* DOWN */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
    ATYPE(STYPE_EBULLET),
    ADELTA(0, 25),
    ASIZE(0, 24, 32, 32),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
    ADELTA(0, 25),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
    ADELTA(0, 15),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),

    /* LEFT */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(-3, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
    ADELTA(-18, 0),
    ATYPE(STYPE_EBULLET),
    ASIZE(-29, 4, 51, 26),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
    ADELTA(-18, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
    ADELTA(-18, 0),
    AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
    /* UP */
    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
    ADELTA(0, 6),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
    ATYPE(STYPE_EBULLET),
    ADELTA(0, 10),
    ASIZE(0, 0, 32, 52),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
    ADELTA(0, 0),
    ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),

    ATYPE(STYPE_ENEMY),
    ASIZE(0, 0, 32, 16),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  AEND,
};

static ANIMSCRIPT hitSpellUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* attackRoundAnimations[] = {
  attackRoundUpAnimation,
  attackRoundDownAnimation,
  attackRoundLeftAnimation,
  attackRoundRightAnimation
};
static ANIMSCRIPT* attackDoubleAnimations[] = {
  attackDoubleUpAnimation,
  attackDoubleDownAnimation,
  attackDoubleLeftAnimation,
  attackDoubleRightAnimation
};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};
static ANIMSCRIPT* hitSpellAnimations[] = {hitSpellUpAnimation, hitSpellDownAnimation, hitSpellLeftAnimation, hitSpellRightAnimation};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GOrcProcess::GOrcProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aIp, aX, aY, ORC_SLOT, aParams, VELOCITY, ATTR_ORC) {
  mSprite->Name("ENEMY ORC");
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_ORC_BMP_SPRITES);
  mSprite->cy = 4;
  SetStatMultipliers(4.0, 3.0, 4.0);
  mRangeX = 24;
  mRangeY = 16;
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->ResetShadow();
}

GOrcProcess::~GOrcProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GOrcProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GOrcProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GOrcProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(3, 7)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GOrcProcess::Attack(DIRECTION aDirection) {
  TInt attackType = Random() & TUint8(3);

//  attackType = 1;
  // 25% chance for either round or double attack
  if (attackType < 2) {
    mSprite->StartAnimationInDirection(attackAnimations, aDirection);
  } else if (attackType == 2) {
    mSprite->StartAnimationInDirection(attackRoundAnimations, aDirection);
  } else {
    mSprite->StartAnimationInDirection(attackDoubleAnimations, aDirection);
  }
}

void GOrcProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GOrcProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitSpellAnimations, aDirection);
}

void GOrcProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}
