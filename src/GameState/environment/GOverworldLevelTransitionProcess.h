#ifndef MODITE_GOVERWORLDLEVELTRANSITIONPROCESS_H
#define MODITE_GOVERWORLDLEVELTRANSITIONPROCESS_H


#include "GEnvironmentProcess.h"

class GOverworldLevelTransitionProcess : public GEnvironmentProcess {
public:
    GOverworldLevelTransitionProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aLevel, TFloat aX, TFloat aY, const char *aKind);

    ~GOverworldLevelTransitionProcess();

public:
    TBool RunBefore();

    TBool RunAfter();

protected:
    GAnchorSprite *mSprite1;
    GAnchorSprite *mSprite2;
    DIRECTION mDirection; // UP or DOWN
    TInt mLevel;          // target level (where stairs lead)
};
#endif //MODITE_GOVERWORLDLEVELTRANSITIONPROCESS_H
