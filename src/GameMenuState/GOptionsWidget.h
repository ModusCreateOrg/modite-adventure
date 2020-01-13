#ifndef GENUS_GOPTIONSWIDGET_H
#define GENUS_GOPTIONSWIDGET_H

#include "Game.h"

class GGameMenuContainer;

class GOptionsWidget : public GButtonWidget {
public:
    GOptionsWidget(GGameMenuContainer *aContainer, TBool aExit);
    ~GOptionsWidget();
    TInt Render(TInt aX, TInt aY);
    void Select();

public:
    GGameMenuContainer *mContainer;
    TBool mExit;
};

#endif //GENUS_GOPTIONSWIDGET_H
