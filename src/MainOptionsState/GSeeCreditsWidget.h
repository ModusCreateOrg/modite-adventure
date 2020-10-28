#ifndef MODITE_GSEECREDITSWIDGET_H
#define MODITE_GSEECREDITSWIDGET_H


#include "Game.h"

class GSeeCreditsWidget : public GButtonWidget {
public:
    GSeeCreditsWidget();
    ~GSeeCreditsWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};


#endif //MODITE_GSEECREDITSWIDGET_H
