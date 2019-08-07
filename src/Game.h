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

// special characters
#define STR_LEFT_ARROW "\xf"
#define STR_RIGHT_ARROW "\xe"

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>

#include "GResources.h"
#include "GGame.h"
#include "GDialogWidget.h"
//#include "GSoundPlayer.h"

// GGameEngine states
#include "GSplashState.h"
#include "GMainMenuState.h"
#include "GGameState.h"

extern BViewPort *gViewPort;
extern BGameEngine *gGameEngine;

#ifdef __XTENSA__
static const TInt MAX_BRIGHTNESS = 0x1fff;
static const TInt MIN_BRIGHTNESS = 0x50;
#endif

#endif //MODITE_GAME_H
