#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

#include "Game.h"
#include "GGamePlayfieldConstants.h"





class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId);

  ~GGamePlayfield();
public:
  TBool IsWall(TFloat aWorldX, TFloat aWorldY) {
    const TUint32 cell = GetCell(aWorldX, aWorldY);
    const TUint16 shifted = cell >> TUint8(16);
    return shifted == ATTR_WALL;
  }
};


#endif //MODITE_GGAMEPLAYFIELD_H
