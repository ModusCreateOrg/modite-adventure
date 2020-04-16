#ifndef GENUS_GDIALOGWIDGET_H
#define GENUS_GDIALOGWIDGET_H

#include "Game.h"

const TUint REPEAT_INPUT_DELAY = 0.5 * FRAMES_PER_SECOND;
const TUint REPEAT_INPUT_INTERVAL = 0.15 * FRAMES_PER_SECOND;

class GDialogWidget : public BDialogWidget {
public:
  GDialogWidget(char *aTitle, TInt aX, TInt aY);
  GDialogWidget(const char *aTitle, TInt aX, TInt aY);
  virtual ~GDialogWidget();
  TBool OnNavigate(TUint16 bits);

  void Run();

private:
  TUint mTimer;
};

#endif //GENUS_GDIALOGWIDGET_H
