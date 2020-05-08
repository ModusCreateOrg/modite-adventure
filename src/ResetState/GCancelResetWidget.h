#ifndef GENUS_GCANCELRESETWIDGET_H
#define GENUS_GCANCELRESETWIDGET_H

#include "Widgets.h"

class GCancelResetWidget : public BButtonWidget {
public:
    GCancelResetWidget();
    ~GCancelResetWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GCANCELRESETWIDGET_H
