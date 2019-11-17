#ifndef GENUS_GHEALTHWIDGET_H
#define GENUS_GHEALTHWIDGET_H

#include "GSoundSliderWidget.h"

class GHealthWidget : public GSoundSliderWidget {
public:
    GHealthWidget();
    ~GHealthWidget();
public:
    TInt Render(TInt aX, TInt aY);
    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse);
    void Select(TInt aVal);
};

#endif //GENUS_GHEALTHWIDGET_H
