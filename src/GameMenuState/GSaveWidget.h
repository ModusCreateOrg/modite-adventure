#ifndef GENUS_GSAVEWIDGET_H
#define GENUS_GSAVEWIDGET_H

#include "Game.h"

class GSaveWidget : public GButtonWidget {
public:
    GSaveWidget();
    ~GSaveWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GSAVEWIDGET_H
