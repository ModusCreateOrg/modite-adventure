#ifndef GENUS_GHEALTHWIDGET_H
#define GENUS_GHEALTHWIDGET_H

#include "GSoundSliderWidget.h"

class GHealthWidget : public GSoundSliderWidget {
public:
    GHealthWidget();
    ~GHealthWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse) OVERRIDE;
    void Select(TInt aVal) OVERRIDE;
};

#endif //GENUS_GHEALTHWIDGET_H
