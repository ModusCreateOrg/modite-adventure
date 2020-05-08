#ifndef GENUS_GDEBUGWIDGET_H
#define GENUS_GDEBUGWIDGET_H

#include "GGame.h"
#include "common/GButtonWidget.h"

class GDebugWidget : public GButtonWidget {
public:
    GDebugWidget();
    ~GDebugWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GDEBUGWIDGET_H
