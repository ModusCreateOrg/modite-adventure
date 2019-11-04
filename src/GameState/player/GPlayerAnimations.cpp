#include "Game.h"
#include "GPlayerAnimations.h"

const TInt16 HITSPEED_FIRST = TInt(1.5 * HITSPEED);

/*
 __  __             _        ____             _ _  
|  \/  | __ _  __ _(_) ___  / ___| _ __   ___| | | 
| |\/| |/ _` |/ _` | |/ __| \___ \| '_ \ / _ \ | | 
| |  | | (_| | (_| | | (__   ___) | |_) |  __/ | | 
|_|  |_|\__,_|\__, |_|\___| |____/| .__/ \___|_|_| 
              |___/               |_|              
*/

const TInt16 SPELL_SPEED = 3;

ANIMSCRIPT spell1Animation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(SPELL_SPEED, IMG_IDLE + 0),
  ASTEP(SPELL_SPEED, IMG_IDLE + 1),
  ASTEP(SPELL_SPEED, IMG_IDLE + 2),
  ASTEP(SPELL_SPEED, IMG_IDLE + 5),
  ASTEP(SPELL_SPEED, IMG_IDLE + 6),
  AEND,
};

ANIMSCRIPT spell2Animation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(SPELL_SPEED, IMG_IDLE + 2),
  ASTEP(SPELL_SPEED, IMG_IDLE + 1),
  ASTEP(SPELL_SPEED, IMG_IDLE + 0),
  ASTEP(SPELL_SPEED, IMG_IDLE + 8),
  ASTEP(SPELL_SPEED, IMG_IDLE + 0),
  AEND,
};

ANIMSCRIPT spellOverlayAnimation[] = {
  ABITMAP(PLAYER_SPELL_SLOT),
  ASTEP(SPELL_SPEED, IMG_SPELL + 0),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 4),
  ASTEP(SPELL_SPEED, IMG_SPELL + 5),
  ASTEP(SPELL_SPEED, IMG_SPELL + 6),
  ASTEP(SPELL_SPEED, IMG_SPELL + 7),
  ASTEP(SPELL_SPEED, IMG_SPELL + 8),
  AEND,
};

/*
 ____       _       _      ____       _   _              
|  _ \ _ __(_)_ __ | | __ |  _ \ ___ | |_(_) ___  _ __   
| | | | '__| | '_ \| |/ / | |_) / _ \| __| |/ _ \| '_ \  
| |_| | |  | | | | |   <  |  __/ (_) | |_| | (_) | | | | 
|____/|_|  |_|_| |_|_|\_\ |_|   \___/ \__|_|\___/|_| |_| 
*/

const TInt16 QUAFF_SPEED = 3;

ANIMSCRIPT quaff1Animation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 1),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 2),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 5),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 6),
  AEND,
};

ANIMSCRIPT quaff2Animation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 2),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 1),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 8),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 0),
  AEND,
};

ANIMSCRIPT quaffOverlayAnimation[] = {
  ABITMAP(PLAYER_HEAL_SLOT),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 0),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 1),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 2),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 3),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 4),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 5),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 6),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 7),
  ASTEP(QUAFF_SPEED, IMG_HEAL + 8),
  AEND,
};

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
  ALOOP,
};

ANIMSCRIPT selectedAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(40, IMG_SELECTED),
  ASTEP(4, IMG_SELECTED + 1),
  ASTEP(40, IMG_SELECTED + 2),
  ASTEP(4, IMG_SELECTED + 1),
  ALOOP,
};

ANIMSCRIPT fallAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(40, IMG_SELECTED),
  ASTEP(4, IMG_SELECTED + 1),
  ASTEP(40, IMG_SELECTED + 2),
  ASTEP(4, IMG_SELECTED + 1),
  AEND,
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
  AEND,
};

ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(1, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 1),
  ADELTA(-3, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 2),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 0),
  AEND,
};

ANIMSCRIPT swordDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN),

  // this is the frame to collide with enemy to hit it
  ADELTA(-2, 22),
  ATYPE(STYPE_PBULLET),
  ASIZE(0, 22, 32, 34),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 1),

  // after the hit frame, reset type to STYPE_PLAYER
  ADELTA(-3, 22),
  ASIZE(7, 0, 26, 16),
  ATYPE(STYPE_PLAYER),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 2),

  ADELTA(-2, 7),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordDownNoBulletAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN),

  // this is the frame to collide with enemy to hit it
  ADELTA(-2, 22),
  //  ATYPE(STYPE_PBULLET),
  ASIZE(0, 22, 32, 34),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 1),

  // after the hit frame, reset type to STYPE_PLAYER
  ADELTA(-3, 22),
  ASIZE(7, 0, 26, 16),
  //  ATYPE(STYPE_PLAYER),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 2),

  ADELTA(-2, 7),
  ASTEP(SWORDSPEED, IMG_SWORD_DOWN + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT hitLightDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_DOWN + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 3),
  AEND
};

ANIMSCRIPT hitMediumDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_DOWN + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 3),
  AEND
};

ANIMSCRIPT hitHardDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_DOWN + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_DOWN + 3),
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
  ADELTA(-10, 0),
  AFLIP(1, IMG_WALK_RIGHT),
  AEND,
};

ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0, 0),
  AFLIP(WALKSPEED, IMG_WALK_RIGHT + 1),
  ADELTA(-4, 0),
  AFLIP(WALKSPEED, IMG_WALK_RIGHT + 2),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-6, 0),
  AFLIP(WALKSPEED, IMG_WALK_RIGHT + 3),
  ADELTA(-4, 0),
  AFLIP(WALKSPEED, IMG_WALK_RIGHT + 0),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-6, 0),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT),

  ADELTA(-18, 0),
  ATYPE(STYPE_PBULLET),
  ASIZE(-37, 4, 41, 24),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 1),
  ADELTA(-18, 0),
  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordLeftNoBulletAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-6, 0),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT),

  ADELTA(-18, 0),
  //  ATYPE(STYPE_PBULLET),
  ASIZE(-37, 4, 41, 24),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 1),
  ADELTA(-18, 0),
  //  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(SWORDSPEED, IMG_SWORD_RIGHT + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT hitLightLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
  AEND
};

ANIMSCRIPT hitMediumLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
  AEND
};

ANIMSCRIPT hitHardLeftAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
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
  ASTEP(1, IMG_WALK_RIGHT),
  AEND,
};

ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT),
  ADELTA(4, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 1),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(6, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 2),
  ADELTA(4, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(8, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT),

  ADELTA(20, 0),
  ATYPE(STYPE_PBULLET),
  ASIZE(22, 4, 41, 24),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 1),

  ADELTA(20, 0),
  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 2),
  ADELTA(11, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordRightNoBulletAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(8, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT),

  ADELTA(20, 0),
  //  ATYPE(STYPE_PBULLET),
  ASIZE(22, 4, 41, 24),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 1),

  ADELTA(20, 0),
  //  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 2),
  ADELTA(11, 0),
  ASTEP(SWORDSPEED, IMG_SWORD_RIGHT + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT hitLightRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
  AEND
};

ANIMSCRIPT hitMediumRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
  AEND
};

ANIMSCRIPT hitHardRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_RIGHT + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_RIGHT + 3),
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
  AEND,
};

ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-2, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 1),
  ADELTA(-7, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 2),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 3),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_UP + 0),
  AEND,
};

ANIMSCRIPT swordUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(4, -2),
  ASTEP(SWORDSPEED, IMG_SWORD_UP),

  ADELTA(2, -2),
  ATYPE(STYPE_PBULLET),
  ASIZE(0, -20, 32, 34),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 1),

  ADELTA(4, -2),
  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 2),

  ADELTA(-2, -2),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT swordUpNoBulletAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(4, -2),
  ASTEP(SWORDSPEED, IMG_SWORD_UP),

  ADELTA(2, -2),
  //  ATYPE(STYPE_PBULLET),
  ASIZE(0, -20, 32, 34),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 1),

  ADELTA(4, -2),
  //  ATYPE(STYPE_PLAYER),
  ASIZE(7, 0, 26, 16),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 2),

  ADELTA(-2, -2),
  ASTEP(SWORDSPEED, IMG_SWORD_UP + 3),
  ADELTA(0, 0),
  AEND,
};

ANIMSCRIPT hitLightUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_UP + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 2),
  AFLIP(HITSPEED, IMG_DAMAGE_UP + 3),
  AEND
};

ANIMSCRIPT hitMediumUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_UP + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 3),
  AEND
};

ANIMSCRIPT hitHardUpAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(HITSPEED_FIRST, IMG_DAMAGE_UP + 3),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 0),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 1),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 2),
  ASTEP(HITSPEED, IMG_DAMAGE_UP + 3),
  AEND
};
