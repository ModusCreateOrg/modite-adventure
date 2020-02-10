#ifndef MODITE_GWIZARDDECOYPROCESS_H
#define MODITE_GWIZARDDECOYPROCESS_H


#include <GameState/GProcess.h>
#include "GWizardProcess.h"

class GWizardDecoyProcess : public GProcess {
public:
  GWizardDecoyProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY, TUint16 aSpriteSheet);
  ~GWizardDecoyProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GWizardProcess *mParent;
};


#endif //MODITE_GWIZARDDECOYPROCESS_H
