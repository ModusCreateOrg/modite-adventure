#include "Game.h"
#include "GStatProcess.h"
#include "GStatSprite.h"

const TInt TIMEOUT = FRAMES_PER_SECOND;

GStatProcess::GStatProcess(GAnchorSprite *aTarget, STAT_TYPE aStatType, const char *aFmt, ...) : GProcess(ATTR_GONE) {
  va_list args;
  va_start(args, aFmt);
  char msg[4096];
  TInt messageLength = vsprintf(msg, aFmt, args);
  mImageNumber = 0;
  //  printf("GStatProcess(%f,%f) %s\n", aX, aY, aMessage);
  mSprite = new GStatSprite(aStatType, msg, mImageNumber);
  TPoint p = aTarget->Center();
  mSprite->x = p.x + (TFloat) (Random() % 16) - 7; // slight random deviation
  mSprite->y = p.y + (TFloat) (Random() % 8);
  mSprite->x -= (TFloat) (messageLength * 5 + 3); // center alignment
  mSprite->vy = (TFloat) -1 / FACTOR;
  mSprite->SetFlags(SFLAG_RENDER | SFLAG_MOVE);
  gGame->CurrentState()->AddSprite(mSprite);
  mTimeout = TIMEOUT;
  va_end(args);
}

GStatProcess::GStatProcess(GAnchorSprite *aTarget, STAT_TYPE aStatType, TInt aImageNumber, TInt aTimeout, const char *aFmt, ...) :
    GStatProcess(aTarget, aStatType, aFmt) {
  if (aImageNumber) {
    mSprite->mImageNumber = mImageNumber = aImageNumber;
    mSprite->x -= 16;
  }
  mTimeout = aTimeout;
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
