#ifndef MODITE_GPLAYERANIMATIONS_H
#define MODITE_GPLAYERANIMATIONS_H

const TInt16 WALKSPEED = 5 * FACTOR;
const TInt16 SWORDSPEED = 3 * FACTOR;
const TInt16 HITSPEED = 2 * FACTOR;

const TFloat VELOCITY = 3 / TFloat(FACTOR);

extern ANIMSCRIPT idleAnimation[];

extern ANIMSCRIPT selectedAnimation[];
extern ANIMSCRIPT fallAnimation[];

// DOWN
extern ANIMSCRIPT idleDownAnimation[];
extern ANIMSCRIPT walkDownAnimation1[];
extern ANIMSCRIPT walkDownAnimation2[];
extern ANIMSCRIPT swordDownAnimation[]; // attack
extern ANIMSCRIPT hitLightDownAnimation[]; // attacked
extern ANIMSCRIPT hitMediumDownAnimation[]; // attacked
extern ANIMSCRIPT hitHardDownAnimation[]; // attacked

// LEFT
extern ANIMSCRIPT idleLeftAnimation[];
extern ANIMSCRIPT walkLeftAnimation1[];
extern ANIMSCRIPT walkLeftAnimation2[];
extern ANIMSCRIPT swordLeftAnimation[]; // attack
extern ANIMSCRIPT hitLightLeftAnimation[]; // attacked
extern ANIMSCRIPT hitMediumLeftAnimation[]; // attacked
extern ANIMSCRIPT hitHardLeftAnimation[]; // attacked

// RIGHT
extern ANIMSCRIPT idleRightAnimation[];
extern ANIMSCRIPT walkRightAnimation1[];
extern ANIMSCRIPT walkRightAnimation2[];
extern ANIMSCRIPT swordRightAnimation[]; // attack
extern ANIMSCRIPT hitLightRightAnimation[]; // attacked
extern ANIMSCRIPT hitMediumRightAnimation[]; // attacked
extern ANIMSCRIPT hitHardRightAnimation[]; // attacked

// UP
extern ANIMSCRIPT idleUpAnimation[];
extern ANIMSCRIPT walkUpAnimation1[];
extern ANIMSCRIPT walkUpAnimation2[];
extern ANIMSCRIPT swordUpAnimation[]; // attack
extern ANIMSCRIPT hitLightUpAnimation[]; // attacked
extern ANIMSCRIPT hitMediumUpAnimation[]; // attacked
extern ANIMSCRIPT hitHardUpAnimation[]; // attacked

#endif // MODITE_GPLAYERANIMATIONS_H
