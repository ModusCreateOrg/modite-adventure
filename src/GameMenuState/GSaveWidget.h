#ifndef GENUS_GSAVEWIDGET_H
#define GENUS_GSAVEWIDGET_H

#include "Game.h"

class GSaveWidget : public GButtonWidget {
public:
    GSaveWidget(GGameState *aGameState);
    ~GSaveWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
protected:
    GGameState *mGameState;
};

#endif //GENUS_GSAVEWIDGET_H
