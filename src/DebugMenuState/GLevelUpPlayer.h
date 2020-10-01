#ifndef MODITE_GLevelUpPlayer_H
#define MODITE_GLevelUpPlayer_H

#include "common/GButtonWidget.h"

class GLevelUpPlayer : public GButtonWidget {
public:
    GLevelUpPlayer();
    ~GLevelUpPlayer() = default;

    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};


#endif //MODITE_GLevelUpPlayer_H
