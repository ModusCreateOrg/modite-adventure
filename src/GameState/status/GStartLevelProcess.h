#ifndef MODITE_GSTARTLEVELPROCESS_H
#define MODITE_GSTARTLEVELPROCESS_H

#include "BProcess.h"

class GStartLevelProcess : public BProcess {
public:
  GStartLevelProcess(const char *aName, TInt aLevel);
  ~GStartLevelProcess();

public:
  TBool RunBefore() {
    return ETrue;
  }
  TBool RunAfter();

protected:
  TInt mX;
  char mText[128];
  TInt mTimer;
};

#endif //MODITE_GSTARTLEVELPROCESS_H
