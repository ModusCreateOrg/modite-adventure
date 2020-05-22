#ifndef GENUS_GRESETWIDGET_H
#define GENUS_GRESETWIDGET_H

#include "Game.h"

class GResetWidget : public GButtonWidget {
public:
    GResetWidget();
    ~GResetWidget() OVERRIDE;
    TInt Render(TInt aX, TInt aY) OVERRIDE;
};

#endif //GENUS_GRESETWIDGET_H
