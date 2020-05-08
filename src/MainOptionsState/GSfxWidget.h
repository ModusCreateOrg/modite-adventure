#ifndef GENUS_GSFXWIDGET_H
#define GENUS_GSFXWIDGET_H

#include "GSoundSliderWidget.h"

class GSfxWidget : public GSoundSliderWidget {
public:
    GSfxWidget();
    ~GSfxWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select(TInt aVal) OVERRIDE;
};

#endif //GENUS_GSFXWIDGET_H
