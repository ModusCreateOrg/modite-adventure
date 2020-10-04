#ifndef GENUS_GHEALTHWIDGET_H
#define GENUS_GHEALTHWIDGET_H

#include "common/GButtonWidget.h"

class GHealthWidget : public GButtonWidget {
public:
    GHealthWidget();
    ~GHealthWidget() OVERRIDE = default;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GHEALTHWIDGET_H
