#ifndef GSTAT_PROCESS
#define GSTAT_PROCESS

#include "GStatSprite.h"
#include "GProcess.h"

class GStatSprite;

class GStatProcess : public GProcess {
public:
  GStatProcess(GAnchorSprite *aTarget, STAT_TYPE aStatType, TInt aImageNumber, TInt aTimeout, const char *aFmt, ...);
  GStatProcess(GAnchorSprite *aTarget, STAT_TYPE aStatType, const char *aFmt, ...);

  ~GStatProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  GStatSprite *mSprite;
  TInt        mTimeout;
  TInt        mImageNumber;
};

#endif

