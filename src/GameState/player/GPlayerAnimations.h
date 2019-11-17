#ifndef MODITE_GPLAYERANIMATIONS_H
#define MODITE_GPLAYERANIMATIONS_H

const TInt16 WALKSPEED = 3 * FACTOR;
const TInt16 SWORDSPEED = 3 * FACTOR;
const TInt16 HITSPEED = 2 * FACTOR;

extern ANIMSCRIPT idleAnimation[];

extern ANIMSCRIPT selectedAnimation[];
extern ANIMSCRIPT fallAnimation[];

// HEAL
extern ANIMSCRIPT quaff1Animation[];
extern ANIMSCRIPT quaff2Animation[];
extern ANIMSCRIPT quaffOverlayAnimation[];

// SPELL
extern ANIMSCRIPT spell1Animation[];
extern ANIMSCRIPT spell2Animation[];
extern ANIMSCRIPT spellOverlayAnimation[];

// DOWN
extern ANIMSCRIPT idleDownAnimation[];
extern ANIMSCRIPT walkDownAnimation1[];
extern ANIMSCRIPT walkDownAnimation2[];
extern ANIMSCRIPT skidDownAnimation[];
extern ANIMSCRIPT swordDownAnimation[]; // attack
extern ANIMSCRIPT swordDownNoBulletAnimation[]; // attack next to wall
extern ANIMSCRIPT hitLightDownAnimation[]; // attacked
extern ANIMSCRIPT hitMediumDownAnimation[]; // attacked
extern ANIMSCRIPT hitHardDownAnimation[]; // attacked

// LEFT
extern ANIMSCRIPT idleLeftAnimation[];
extern ANIMSCRIPT walkLeftAnimation1[];
extern ANIMSCRIPT walkLeftAnimation2[];
extern ANIMSCRIPT skidLeftAnimation[];
extern ANIMSCRIPT swordLeftAnimation[]; // attack
extern ANIMSCRIPT swordLeftNoBulletAnimation[]; // attack next to wall
extern ANIMSCRIPT hitLightLeftAnimation[]; // attacked
extern ANIMSCRIPT hitMediumLeftAnimation[]; // attacked
extern ANIMSCRIPT hitHardLeftAnimation[]; // attacked

// RIGHT
extern ANIMSCRIPT idleRightAnimation[];
extern ANIMSCRIPT walkRightAnimation1[];
extern ANIMSCRIPT walkRightAnimation2[];
extern ANIMSCRIPT skidRightAnimation[];
extern ANIMSCRIPT swordRightAnimation[]; // attack
extern ANIMSCRIPT swordRightNoBulletAnimation[]; // attack next to wall
extern ANIMSCRIPT hitLightRightAnimation[]; // attacked
extern ANIMSCRIPT hitMediumRightAnimation[]; // attacked
extern ANIMSCRIPT hitHardRightAnimation[]; // attacked

// UP
extern ANIMSCRIPT idleUpAnimation[];
extern ANIMSCRIPT walkUpAnimation1[];
extern ANIMSCRIPT walkUpAnimation2[];
extern ANIMSCRIPT skidUpAnimation[];
extern ANIMSCRIPT swordUpAnimation[]; // attack
extern ANIMSCRIPT swordUpNoBulletAnimation[]; // attack next to wall
extern ANIMSCRIPT hitLightUpAnimation[]; // attacked
extern ANIMSCRIPT hitMediumUpAnimation[]; // attacked
extern ANIMSCRIPT hitHardUpAnimation[]; // attacked

#endif // MODITE_GPLAYERANIMATIONS_H
