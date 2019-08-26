#include "Game.h"
#include "GStatProcess.h"
#include "GStatSprite.h"

const TInt TIMEOUT = 200;

GStatProcess::GStatProcess(TFloat aX, TFloat aY, const char *aMessage)
    : BProcess() {
//  printf("GStatProcess(%f,%f) %s\n", aX, aY, aMessage);
  mSprite = new GStatSprite(STAT_SIZE_16x16, aMessage);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->vy = -.6;
  mSprite->flags |= SFLAG_RENDER | SFLAG_MOVE;
  gGameEngine->AddSprite(mSprite);
  mTimeout = TIMEOUT;
}

GStatProcess::~GStatProcess() {
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

TBool GStatProcess::RunBefore() { return ETrue; }

TBool GStatProcess::RunAfter() {
  const TFloat screenY = mSprite->y - gViewPort->mRect.y1;
  if (screenY < 0) {
    return EFalse;
  }

  if (--mTimeout > 0) {
    return ETrue;
  }

  return EFalse;
}
