#ifndef GENUS_GDEBUGWIDGET_H
#define GENUS_GDEBUGWIDGET_H

#include "Game.h"

class GDebugWidget : public GButtonWidget {
public:
    GDebugWidget();
    ~GDebugWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GDEBUGWIDGET_H
