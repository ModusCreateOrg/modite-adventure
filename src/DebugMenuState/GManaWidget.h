#ifndef GENUS_GMANAWIDGET_H
#define GENUS_GMANAWIDGET_H

#include "GSoundSliderWidget.h"

class GManaWidget : public GButtonWidget {
public:
    GManaWidget();
    ~GManaWidget() OVERRIDE = default;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GMANAWIDGET_H
