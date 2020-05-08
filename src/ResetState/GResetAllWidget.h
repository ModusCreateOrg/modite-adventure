#ifndef GENUS_GRESETALLWIDGET_H
#define GENUS_GRESETALLWIDGET_H

#include "Widgets.h"

class GResetAllWidget : public BButtonWidget {
public:
    GResetAllWidget();
    ~GResetAllWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GRESETALLWIDGET_H
