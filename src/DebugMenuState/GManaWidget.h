#ifndef GENUS_GMANAWIDGET_H
#define GENUS_GMANAWIDGET_H

#include "GSoundSliderWidget.h"

class GManaWidget : public GSoundSliderWidget {
public:
    GManaWidget();
    ~GManaWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse) OVERRIDE;
    void Select(TInt aVal) OVERRIDE;
};

#endif //GENUS_GMANAWIDGET_H
