//
// Created by mschwartz on 7/3/19.
//

#ifndef MODITE_GAME_H
#define MODITE_GAME_H

#define FRAME_RATE_INFO 0
#undef FRAME_RATE_INFO

#define ENABLE_AUDIO
#undef ENABLE_AUDIO

#define ENABLE_OPTIONS
#undef ENABLE_OPTIONS

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>

#include "GResources.h"
#include "GGame.h"
//#include "GSoundPlayer.h"
#include "GSplashState.h"
#include "GTitleState.h"
#include "GGameState.h"

extern BViewPort *gViewPort;
extern BGameEngine *gGameEngine;


#endif //MODITE_GAME_H
