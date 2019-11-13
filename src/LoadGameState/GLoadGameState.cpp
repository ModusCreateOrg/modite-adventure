#include "GLoadGameState.h"
#include "GLoadGamePlayfield.h"
#include "GGame.h"
#include "common/GSavedGameList.h"

GLoadGameState::GLoadGameState() : BGameEngine(gViewPort) {
  mPlayfield = new GLoadGamePlayfield();
  gViewPort->SetRect(TRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gSavedGameList.LoadSavedGameList();
  gSavedGameList.Dump();
}

GLoadGameState::~GLoadGameState() {
  if (mPlayfield) {
    delete mPlayfield;
    mPlayfield = ENull;
  }
}

void GLoadGameState::PreRender() {
}

void GLoadGameState::PostRender() {
}
