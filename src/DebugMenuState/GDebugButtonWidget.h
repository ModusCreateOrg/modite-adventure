#ifndef GENUS_GDEBUGBUTTONWIDGET_H
#define GENUS_GDEBUGBUTTONWIDGET_H

#include "Game.h"
#include <common/GButtonWidget.h>

class GDebugMenuContainer;

class GDebugButtonWidget : public GButtonWidget {
public:
    GDebugButtonWidget(const char *mText, TInt aState, GDebugMenuContainer *aContainer);
    ~GDebugButtonWidget();
    TInt Render(TInt aX, TInt aY);
    void Select();

public:
    GDebugMenuContainer *mContainer;
};

#endif //GENUS_GDEBUGBUTTONWIDGET_H
