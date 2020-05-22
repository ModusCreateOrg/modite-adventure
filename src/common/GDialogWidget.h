#ifndef GENUS_GDIALOGWIDGET_H
#define GENUS_GDIALOGWIDGET_H

//#include "Game.h"
#include <BDialogWidget.h>

class GDialogWidget : public BDialogWidget {
public:
  GDialogWidget(char *aTitle, TInt aX, TInt aY);
  GDialogWidget(const char *aTitle, TInt aX, TInt aY);
  ~GDialogWidget() OVERRIDE;
  TBool OnNavigate(TUint16 bits) OVERRIDE;

  void Run() OVERRIDE;

private:
  TUint mTimer;
};

#endif //GENUS_GDIALOGWIDGET_H
