#ifndef GENUS_GCANCELRESETWIDGET_H
#define GENUS_GCANCELRESETWIDGET_H

#include "Game.h"

class GCancelResetWidget : public GButtonWidget {
public:
    GCancelResetWidget();
    ~GCancelResetWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GCANCELRESETWIDGET_H
