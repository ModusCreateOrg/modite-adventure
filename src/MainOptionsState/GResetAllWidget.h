#ifndef GENUS_GRESETALLWIDGET_H
#define GENUS_GRESETALLWIDGET_H

#include "Game.h"

class GResetAllWidget : public GButtonWidget {
public:
    GResetAllWidget();
    ~GResetAllWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GRESETALLWIDGET_H
