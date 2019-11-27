#ifndef GENUS_GMANAWIDGET_H
#define GENUS_GMANAWIDGET_H

#include "GSoundSliderWidget.h"

class GManaWidget : public GSoundSliderWidget {
public:
    GManaWidget();
    ~GManaWidget();
public:
    TInt Render(TInt aX, TInt aY);
    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse);
    void Select(TInt aVal);
};

#endif //GENUS_GMANAWIDGET_H
