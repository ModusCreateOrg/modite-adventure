#ifndef GSTAT_PROCESS
#define GSTAT_PROCESS

#include "BProcess.h"

class GStatSprite;

class GStatProcess : public BProcess {
public:
  GStatProcess(TFloat aX, TFloat aY, const char *aFmt, ...);

  ~GStatProcess();

public:
  void SetImageNumber(TInt aImageNumber);
  void SetTimeout(TInt aTime) { mTimeout = aTime; };

  TBool RunBefore();

  TBool RunAfter();

protected:
  GStatSprite *mSprite;
  TInt        mTimeout;
  TInt        mImageNumber;
};

#endif

