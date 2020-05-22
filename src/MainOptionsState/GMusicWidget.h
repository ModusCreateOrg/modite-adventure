#ifndef GENUS_GMUSICWIDGET_H
#define GENUS_GMUSICWIDGET_H

#include "GSoundSliderWidget.h"

class GMusicWidget : public GSoundSliderWidget {
public:
    GMusicWidget();
    ~GMusicWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select(TInt aVal) OVERRIDE;
};

#endif //GENUS_GMUSICWIDGET_H
