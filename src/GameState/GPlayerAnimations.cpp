#include "Game.h"
#include "GPlayerAnimations.h"

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */
ANIMSCRIPT idleAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(40, IMG_IDLE),
  ASTEP(4, IMG_IDLE + 1),
  ASTEP(40, IMG_IDLE + 2),
  ASTEP(4, IMG_IDLE + 1),
  ALOOP
};

ANIMSCRIPT selectedAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(40, IMG_SELECTED),
  ASTEP(4, IMG_SELECTED + 1),
  ASTEP(40, IMG_SELECTED + 2),
  ASTEP(4, IMG_SELECTED + 1),
  ALOOP
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(1, IMG_WALK_DOWN),
  AEND
};

ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(1,0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 1),
  ADELTA(-3, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 2),
  AEND
};

ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 0),
  AEND
};

ANIMSCRIPT swordDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0,0),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN),
  ADELTA(-2,22),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 1),
  ADELTA(-3,22),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 2),
  ADELTA(-1,7),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 3),
  AEND
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */
ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(1, IMG_WALK_LEFT_RIGHT),
  AEND
};

ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0,0),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 1),
  ADELTA(-4,0),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 2),
  AEND
};

ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-6,0),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 3),
  ADELTA(-4,0),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 0),
  AEND
};

ANIMSCRIPT swordLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-6,0),
  AFLIP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT),
  ADELTA(-18,0),
  AFLIP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 1),
  ADELTA(-18,0),
  AFLIP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 2),
  ADELTA(-9,0),
  AFLIP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 3),
  AEND
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */
ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(1, IMG_WALK_LEFT_RIGHT),
  AEND
};

ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0,0),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT),
  ADELTA(4,0),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 1),
  AEND
};

ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(6,0),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 2),
  ADELTA(4,0),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 3),
  AEND
};

ANIMSCRIPT swordRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(8,0),
  ASTEP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT),
  ADELTA(20,0),
  ASTEP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 1),
  ADELTA(20,0),
  ASTEP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 2),
  ADELTA(11,0),
  ASTEP(SWORDSPEED, IMG_SWORD_LEFT_RIGHT + 3),
  AEND
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */
ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(1, IMG_WALK_UP),
  AEND
};

ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 1),
  ADELTA(-7, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 2),
  AEND
};

ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 3),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 0),
  AEND
};

ANIMSCRIPT swordUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(2,0),
  ASTEP(SWORDSPEED, IMG_SWORD_UP),
  ADELTA(0,0),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 1),
  ADELTA(2,0),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 2),
  ADELTA(-1,0),
  ASTEP(SWORDSPEED, IMG_WALK_UP + 3),
  AEND
};

