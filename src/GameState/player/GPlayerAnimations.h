#ifndef MODITE_GPLAYERANIMATIONS_H
#define MODITE_GPLAYERANIMATIONS_H

const TInt16 WALKSPEED = 4 * FACTOR;
const TInt16 SWORDSPEED = (TInt16)(3.2 * FACTOR);
const TInt16 SWORDSPEED_WITH_GLOVE = (TInt16)(2.7 * FACTOR);

const TInt16 HITSPEED = 3 * FACTOR;
const TInt16 HITSPEED_FIRST = TInt(1.5 * HITSPEED);
const TInt16 SPELL_SPEED = 1.5 * FACTOR;

extern ANIMSCRIPT idleAnimation[];

extern ANIMSCRIPT selectedAnimation[];
extern ANIMSCRIPT fallAnimation[];
extern ANIMSCRIPT landAnimation[];

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
extern ANIMSCRIPT swordChargeDownAnimation[];
extern ANIMSCRIPT swordDownAnimationNoGloves[]; // attack
extern ANIMSCRIPT swordDownAnimationWithGloves[]; // attack
extern ANIMSCRIPT hitLightDownAnimation[]; // attacked
extern ANIMSCRIPT hitMediumDownAnimation[]; // attacked
extern ANIMSCRIPT hitHardDownAnimation[]; // attacked

// LEFT
extern ANIMSCRIPT idleLeftAnimation[];
extern ANIMSCRIPT walkLeftAnimation1[];
extern ANIMSCRIPT walkLeftAnimation2[];
extern ANIMSCRIPT swordChargeLeftAnimation[];
extern ANIMSCRIPT swordLeftAnimationNoGloves[]; // attack
extern ANIMSCRIPT swordLeftAnimationWithGloves[]; // attack
extern ANIMSCRIPT hitLightLeftAnimation[]; // attacked
extern ANIMSCRIPT hitMediumLeftAnimation[]; // attacked
extern ANIMSCRIPT hitHardLeftAnimation[]; // attacked

// RIGHT
extern ANIMSCRIPT idleRightAnimation[];
extern ANIMSCRIPT walkRightAnimation1[];
extern ANIMSCRIPT walkRightAnimation2[];
extern ANIMSCRIPT swordChargeRightAnimation[];
extern ANIMSCRIPT swordRightAnimationNoGloves[]; // attack
extern ANIMSCRIPT swordRightAnimationWithGloves[]; // attack
extern ANIMSCRIPT hitLightRightAnimation[]; // attacked
extern ANIMSCRIPT hitMediumRightAnimation[]; // attacked
extern ANIMSCRIPT hitHardRightAnimation[]; // attacked

// UP
extern ANIMSCRIPT idleUpAnimation[];
extern ANIMSCRIPT walkUpAnimation1[];
extern ANIMSCRIPT walkUpAnimation2[];
extern ANIMSCRIPT swordChargeUpAnimation[];
extern ANIMSCRIPT swordUpAnimationNoGloves[]; // attack
extern ANIMSCRIPT swordUpAnimationWithGloves[]; // attack
extern ANIMSCRIPT hitLightUpAnimation[]; // attacked
extern ANIMSCRIPT hitMediumUpAnimation[]; // attacked
extern ANIMSCRIPT hitHardUpAnimation[]; // attacked

extern ANIMSCRIPT* idleAnimations[];
extern ANIMSCRIPT* walkAnimations1[];
extern ANIMSCRIPT* walkAnimations2[];
extern ANIMSCRIPT* walkAnimations3[];
extern ANIMSCRIPT* walkAnimations4[];
extern ANIMSCRIPT* swordChargeAnimations[];
extern ANIMSCRIPT* swordAnimationsNoGloves[];
extern ANIMSCRIPT* swordAnimationsWithGloves[];
extern ANIMSCRIPT* hitLightAnimations[];
extern ANIMSCRIPT* hitMediumAnimations[];
extern ANIMSCRIPT* hitHardAnimations[];

#endif // MODITE_GPLAYERANIMATIONS_H
