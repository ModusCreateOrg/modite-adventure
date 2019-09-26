#include "Game.h"
#include "GStatProcess.h"
#include "GStatSprite.h"

const TInt TIMEOUT = 30 * 2;

GStatProcess::GStatProcess(TFloat aX, TFloat aY, const char *aFmt, ...)
    : BProcess() {
  char msg[4096];
  va_list args;
  va_start(args, aFmt);
  vsprintf(msg, aFmt, args);

  mImageNumber = 0;
//  printf("GStatProcess(%f,%f) %s\n", aX, aY, aMessage);
  mSprite = new GStatSprite(STAT_SIZE_16x16, msg, mImageNumber);
  mSprite->x = aX - 64;
  mSprite->y = aY - 64;
  mSprite->vy = -.8;
  mSprite->flags |= SFLAG_RENDER | SFLAG_MOVE;
  gGameEngine->AddSprite(mSprite);
  mTimeout = TIMEOUT;
}

GStatProcess::~GStatProcess() {
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

void GStatProcess::SetImageNumber(TInt aImageNumber) {
  mSprite->mImageNumber = aImageNumber;
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
