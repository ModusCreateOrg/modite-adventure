#ifndef GENUS_GRESETOPTIONSWIDGET_H
#define GENUS_GRESETOPTIONSWIDGET_H

#include "Game.h"

class GResetOptionsWidget : public GButtonWidget {
public:
    GResetOptionsWidget();
    ~GResetOptionsWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GRESETOPTIONSWIDGET_H
