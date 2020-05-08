#ifndef GENUS_GQUITWIDGET_H
#define GENUS_GQUITWIDGET_H

#include "Game.h"

class GQuitWidget : public GButtonWidget {
public:
    GQuitWidget();
    ~GQuitWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GQUITWIDGET_H
