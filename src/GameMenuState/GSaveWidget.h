#ifndef GENUS_GSAVEWIDGET_H
#define GENUS_GSAVEWIDGET_H

#include "Game.h"

class GSaveWidget : public GButtonWidget {
public:
    EXPLICIT GSaveWidget(GGameState *aGameState);
    ~GSaveWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
protected:
    GGameState *mGameState;
};

#endif //GENUS_GSAVEWIDGET_H
