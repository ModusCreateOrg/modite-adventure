#ifndef GENUS_GEXITDUNGEONWIDGET_H
#define GENUS_GEXITDUNGEONWIDGET_H

#include "Game.h"

class GExitDungeonWidget : public GButtonWidget {
public:
    GExitDungeonWidget();
    ~GExitDungeonWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GEXITDUNGEONWIDGET_H
