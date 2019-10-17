#ifndef GENUS_GRESETWIDGET_H
#define GENUS_GRESETWIDGET_H

#include "Game.h"

class GResetWidget : public GButtonWidget {
public:
    GResetWidget();
    ~GResetWidget();
    TInt Render(TInt aX, TInt aY);
};

#endif //GENUS_GRESETWIDGET_H
