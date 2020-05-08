#ifndef GENUS_GBUTTONWIDGET_H
#define GENUS_GBUTTONWIDGET_H

#include <Widgets/BButtonWidget.h>

class GButtonWidget : public BButtonWidget {
public:
    GButtonWidget(const char *aText, TInt aState = -1);
    ~GButtonWidget();

    TInt Render(TInt aX, TInt aY);
    void Select();

    TInt mState;
};

#endif //GENUS_GBUTTONWIDGET_H
