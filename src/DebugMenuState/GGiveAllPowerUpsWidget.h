#ifndef MODITE_GGIVEALLPOWERUPSWIDGET_H
#define MODITE_GGIVEALLPOWERUPSWIDGET_H

#include "common/GButtonWidget.h"

class GGiveAllPowerUpsWidget : public GButtonWidget {
public:
    GGiveAllPowerUpsWidget();
    ~GGiveAllPowerUpsWidget() = default;

    TInt Render(TInt aX, TInt aY) OVERRIDE;
    void Select() OVERRIDE;
};


#endif //MODITE_GGIVEALLPOWERUPSWIDGET_H
