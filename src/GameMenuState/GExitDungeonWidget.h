#ifndef GENUS_GEXITDUNGEONWIDGET_H
#define GENUS_GEXITDUNGEONWIDGET_H

#include "Game.h"

class GExitDungeonWidget : public GButtonWidget {
public:
    GExitDungeonWidget();
    ~GExitDungeonWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GEXITDUNGEONWIDGET_H
