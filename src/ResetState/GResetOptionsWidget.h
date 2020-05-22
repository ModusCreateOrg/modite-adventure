#ifndef GENUS_GRESETOPTIONSWIDGET_H
#define GENUS_GRESETOPTIONSWIDGET_H

#include "Widgets.h"

class GResetOptionsWidget : public BButtonWidget {
public:
    GResetOptionsWidget();
    ~GResetOptionsWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GRESETOPTIONSWIDGET_H
