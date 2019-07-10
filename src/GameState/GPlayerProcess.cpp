#include "GPlayerProcess.h"
#include "GGamePlayfield.h"

enum {
  IDLE_STATE
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(10, 0),
  ASTEP(1, 1),
  ASTEP(10, 2),
  ASTEP(1, 1),
  ALOOP
};

GPlayerProcess::GPlayerProcess(GGameState *aGameState, GGamePlayfield *aPlayfield) {
  mGameState = aGameState;
  mPlayfield = aPlayfield;
  BBitmap *bm = gResourceManager.GetBitmap(PLAYER_SLOT);
  TRGB *pal = bm->GetPalette();
  for (TInt i=128; i<192; i++) {
    gDisplay.SetColor(i, pal[i]);
  }
  mSprite = new BAnimSprite(1, PLAYER_SLOT);
  mSprite->x = mSprite->y = 16;
  mGameState->AddSprite(mSprite);
  mSprite->StartAnimation(idleAnimation);

}

GPlayerProcess::~GPlayerProcess() {
  //
}

#define DELTA 16

TBool GPlayerProcess::IdleState() {
  return ETrue;
}
TBool GPlayerProcess::WalkState() {
  return ETrue;
}
TBool GPlayerProcess::RunBefore() {
  if (gControls.IsPressed(JOYLEFT)) {
    if (gViewPort->mWorldX > 0) {
      gViewPort->mWorldX -= DELTA;
      mSprite->x -= DELTA;
    }
  }
  if (gControls.IsPressed(JOYRIGHT)) {
    if (gViewPort->mWorldX < mPlayfield->MapMaxX()) {
      gViewPort->mWorldX += DELTA;
      mSprite->x += DELTA;
    }
  }
  if (gControls.IsPressed(JOYUP)) {
    if (gViewPort->mWorldY > 0) {
      gViewPort->mWorldY -= DELTA;
      mSprite->y -= DELTA;
    }
  }
  if (gControls.IsPressed(JOYDOWN)) {
    if (gViewPort->mWorldY < mPlayfield->MapMaxY()) {
      gViewPort->mWorldY += DELTA;
      mSprite->y += DELTA;
    }
  }

  return ETrue;
}

TBool GPlayerProcess::RunAfter() {
  return ETrue;
}
