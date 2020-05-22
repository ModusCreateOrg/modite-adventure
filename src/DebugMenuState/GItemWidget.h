#ifndef GENUS_GITEMWIDGET_H
#define GENUS_GITEMWIDGET_H

#include "Game.h"
#include "common/GButtonWidget.h"

class GItemWidget : public GButtonWidget {
public:
    GItemWidget(TInt aItem, BGameEngine *aState);
    ~GItemWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
    void Run() OVERRIDE;

public:
    TInt mItem;
    BGameEngine *mState;
};

#endif //GENUS_GITEMWIDGET_H
