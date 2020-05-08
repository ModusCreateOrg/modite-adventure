#ifndef GENUS_GRESUMEGAMEWIDGET_H
#define GENUS_GRESUMEGAMEWIDGET_H

#include "Game.h"

class GResumeGameWidget : public GButtonWidget {
public:
    GResumeGameWidget();
    ~GResumeGameWidget() OVERRIDE;
    void Select() OVERRIDE;
};

#endif //GENUS_GRESUMEGAMEWIDGET_H
