#ifndef MODITE_GAME_H
#define MODITE_GAME_H

#define FRAME_RATE_INFO 0
#undef FRAME_RATE_INFO

#define ENABLE_AUDIO
//#undef ENABLE_AUDIO

#define ENABLE_OPTIONS
// #undef ENABLE_OPTIONS

// Debug mode
#define DEBUG_MODE
// #undef DEBUG_MODE

// special characters
#define STR_LEFT_ARROW "\xf"
#define STR_RIGHT_ARROW "\xe"

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>

const TUint16 FACTOR = FRAMES_PER_SECOND / 30;

#include "GResources.h"
#include "GGame.h"
#include "common/GDialogWidget.h"
#include "common/GButtonWidget.h"
#include "TOptions.h"
#include "GSoundPlayer.h"

// GGameEngine states
#include "GSplashState.h"
#include "GGameState.h"
#include "GMainMenuState.h"
#include "LoadGameState/GLoadGameState.h"
#include "GMainOptionsState.h"
#include "GResetOptionsState.h"
#include "GCreditsState.h"
#include "GGameMenuState.h"
#include "GVictoryState.h"
#include "DebugMenuState/GDebugMenuState.h"
//#include "GInventory.h"

extern BViewPort   *gViewPort;
extern BGameEngine *gGameEngine;
extern TOptions    *gOptions;
extern BFont *gFont8x8, *gFont16x16;

#ifdef __XTENSA__
static const TInt MAX_BRIGHTNESS = 0x1fff;
static const TInt MIN_BRIGHTNESS = 0x50;
#endif

/**
 * Control bits definitions
 */
const TUint16 CONTROL_JOYUP = JOYUP;
const TUint16 CONTROL_JOYDOWN = JOYDOWN;
const TUint16 CONTROL_JOYLEFT = JOYLEFT;
const TUint16 CONTROL_JOYRIGHT = JOYRIGHT;
const TUint16 CONTROL_FIRE = BUTTONA;
const TUint16 CONTROL_RUN = BUTTONB;
const TUint16 CONTROL_SPELL = BUTTONX;
const TUint16 CONTROL_INVENTORY = BUTTONR;
const TUint16 CONTROL_QUAFF = BUTTONL;
const TUint16 CONTROL_DEBUG = BUTTON_SELECT;


#endif //MODITE_GAME_H
