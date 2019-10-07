#include "Game.h"
#include "GMainMenuProcess.h"
#include "GMainMenuPlayfield.h"

GMainMenuState::GMainMenuState() : BGameEngine(gViewPort) {
  mPlayfield = new GMainMenuPlayfield();
  AddProcess(new GMainMenuProcess());

// TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(MAIN_MENU_XM);
#endif
}

GMainMenuState::~GMainMenuState() {
  delete mPlayfield;
  mPlayfield = ENull;
}

