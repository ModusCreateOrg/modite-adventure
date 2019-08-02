//
// Created by mschwartz on 8/1/19.
//

#ifndef MODITE_GPLAYERANIMATIONS_H
#define MODITE_GPLAYERANIMATIONS_H

const TUint16 WALKSPEED = 5;
const TUint16 SWORDSPEED = 3;
const TFloat VELOCITY = 4;

extern ANIMSCRIPT idleAnimation[];

extern ANIMSCRIPT selectedAnimation[];

// DOWN
extern ANIMSCRIPT idleDownAnimation[];
extern ANIMSCRIPT walkDownAnimation1[];
extern ANIMSCRIPT walkDownAnimation2[];
extern ANIMSCRIPT swordDownAnimation[]; // attack

// LEFT
extern ANIMSCRIPT idleLeftAnimation[];
extern ANIMSCRIPT walkLeftAnimation1[];
extern ANIMSCRIPT walkLeftAnimation2[];
extern ANIMSCRIPT swordLeftAnimation[]; // attack

// RIGHT
extern ANIMSCRIPT idleRightAnimation[];
extern ANIMSCRIPT walkRightAnimation1[];
extern ANIMSCRIPT walkRightAnimation2[];
extern ANIMSCRIPT swordRightAnimation[]; // attack

// UP
extern ANIMSCRIPT idleUpAnimation[];
extern ANIMSCRIPT walkUpAnimation1[];
extern ANIMSCRIPT walkUpAnimation2[];
extern ANIMSCRIPT swordUpAnimation[]; // attack

#endif //MODITE_GPLAYERANIMATIONS_H
