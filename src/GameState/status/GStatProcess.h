#ifndef GSTAT_PROCESS
#define GSTAT_PROCESS

#include "GStatSprite.h"
#include "GProcess.h"

class GStatSprite;

class GStatProcess : public GProcess {
public:
  GStatProcess(BGameEngine *aState, TFloat aX, TFloat aY, const char *aFmt, ...);
  GStatProcess(TFloat aX, TFloat aY, const char *aFmt, ...);

  ~GStatProcess() OVERRIDE;

public:
  void SetImageNumber(TInt aImageNumber);
  void SetMessageType(STAT_TYPE aType);
  void SetTimeout(TInt aTime) { mTimeout = aTime; };

  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  void Init(BGameEngine *aState, TFloat aX, TFloat aY, const char *aFmt, va_list args);

protected:
  GStatSprite *mSprite;
  TInt        mTimeout;
  TInt        mImageNumber;
};

#endif

