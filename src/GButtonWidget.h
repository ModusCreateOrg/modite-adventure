#ifndef GENUS_GBUTTONWIDGET_H
#define GENUS_GBUTTONWIDGET_H

#include "Game.h"

class GButtonWidget : public BButtonWidget {
public:
    GButtonWidget(const char *mText, TUint aState = -1);
    ~GButtonWidget();

    TInt Render(TInt aX, TInt aY);
    void Select();

private:
    TUint mState;
};

#endif //GENUS_GBUTTONWIDGET_H
