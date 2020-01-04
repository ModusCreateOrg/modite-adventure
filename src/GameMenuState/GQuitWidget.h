#ifndef GENUS_GQUITWIDGET_H
#define GENUS_GQUITWIDGET_H

#include "Game.h"

class GQuitWidget : public GButtonWidget {
public:
    GQuitWidget();
    ~GQuitWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GQUITWIDGET_H
