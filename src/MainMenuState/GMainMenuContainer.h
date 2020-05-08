#ifndef GENUS_GMAINMENUCONTAINER_H
#define GENUS_GMAINMENUCONTAINER_H

#include "Game.h"

class GMainMenuProcess;

class GMainMenuContainer : public GDialogWidget {
public:
  GMainMenuContainer(TInt aX, TInt aY, GMainMenuProcess *aProcess);
  ~GMainMenuContainer() OVERRIDE;
  TBool OnNavigate(TUint16 bits) OVERRIDE;

public:
  GMainMenuProcess *mProcess;
};

#endif //GENUS_GMAINMENUCONTAINER_H
