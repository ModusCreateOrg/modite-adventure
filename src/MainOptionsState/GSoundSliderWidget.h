#ifndef GENUS_GSOUNDSLIDERWIDGET_H
#define GENUS_GSOUNDSLIDERWIDGET_H

#include "Widgets.h"

class GSoundSliderWidget : public BSliderWidget {
public:
    GSoundSliderWidget(char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground = -1);

    GSoundSliderWidget(const char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground = -1);
    ~GSoundSliderWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse) OVERRIDE;
    void Set(TInt aVal);
    void Select(TInt aVal) OVERRIDE = 0;
};

#endif //GENUS_GSOUNDSLIDERWIDGET_H
