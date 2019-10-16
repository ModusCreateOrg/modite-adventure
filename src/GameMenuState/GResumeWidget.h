#ifndef GENUS_GRESUMEWIDGET_H
#define GENUS_GRESUMEWIDGET_H

#include "Game.h"

class GResumeWidget : public GButtonWidget {
public:
    GResumeWidget();
    ~GResumeWidget();
public:
    TInt Render(TInt aX, TInt aY);
    void Select();
};

#endif //GENUS_GRESUMEWIDGET_H
