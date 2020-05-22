#ifndef GENUS_GBRIGHTNESSWIDGET_H
#define GENUS_GBRIGHTNESSWIDGET_H

#include "GSoundSliderWidget.h"

class GBrightnessWidget : public GSoundSliderWidget {
public:
    GBrightnessWidget();
    ~GBrightnessWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select(TInt aVal) OVERRIDE;
};

#endif //GENUS_GBRIGHTNESSWIDGET_H
