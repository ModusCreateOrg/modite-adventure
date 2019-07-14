#include "GTitleState.h"
#include "GTitlePlayfield.h"
#include "GTitleProcess.h"

GTitleState::GTitleState() : BGameEngine(gViewPort) {
  mPlayfield = new GTitlePlayfield();
  AddProcess(new GTitleProcess());
}

GTitleState::~GTitleState() {
  // delete mPlayfield
}