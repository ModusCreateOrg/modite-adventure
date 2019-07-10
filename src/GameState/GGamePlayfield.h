#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

#include "Game.h"

class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTilesSlot, TUint16 aMapSlot, TUint16 aCodesSlot);

  ~GGamePlayfield();
};


#endif //MODITE_GGAMEPLAYFIELD_H
