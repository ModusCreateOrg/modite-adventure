#ifndef GENUS_GEXITWIDGET_H
#define GENUS_GEXITWIDGET_H

#include "Game.h"

class GExitWidget : public GButtonWidget {
public:
    GExitWidget();
    ~GExitWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GEXITWIDGET_H
