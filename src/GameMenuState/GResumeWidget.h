#ifndef GENUS_GRESUMEWIDGET_H
#define GENUS_GRESUMEWIDGET_H

#include "Game.h"

class GResumeWidget : public GButtonWidget {
public:
    GResumeWidget();
    ~GResumeWidget() OVERRIDE;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GRESUMEWIDGET_H
