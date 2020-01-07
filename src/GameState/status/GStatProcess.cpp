#include "Game.h"
#include "GStatProcess.h"
#include "GStatSprite.h"

const TInt TIMEOUT = FRAMES_PER_SECOND;

GStatProcess::GStatProcess(TFloat aX, TFloat aY, const char *aFmt, ...) : GProcess(ATTR_GONE) {
  char msg[4096];
  va_list args;
  va_start(args, aFmt);
  vsprintf(msg, aFmt, args);

  mImageNumber = 0;
//  printf("GStatProcess(%f,%f) %s\n", aX, aY, aMessage);
  mSprite = new GStatSprite(STAT_SIZE_16x16, msg, mImageNumber);
  mSprite->x = aX - 56 + Random() % 16; // slight random deviation
  mSprite->y = aY - 60 + Random() % 8;
  mSprite->vy = -1;
  mSprite->SetFlags(SFLAG_RENDER | SFLAG_MOVE);
  gGame->CurrentState()->AddSprite(mSprite);
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

void GStatProcess::SetMessageType(STAT_TYPE aType) {
  mSprite->mMessageType = aType;
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
