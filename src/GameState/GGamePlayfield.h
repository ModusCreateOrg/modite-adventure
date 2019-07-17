#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

#include "Game.h"

// these match the codes set for tiles in Pro Motion:
const TUint16 ATTR_FLOOR_BIT = 0;
const TUint32 ATTR_FLOOR = (ATTR_FLOOR_BIT << 16);

const TUint16 ATTR_WALL_BIT = 1;
const TUint32 ATTR_WALL = (ATTR_WALL_BIT << 16);

const TUint16 ATTR_FALSE_WALL_BIT = 3;
const TUint32 ATTR_FALSE_WALL = (ATTR_FALSE_WALL_BIT << 16);

const TUint16 ATTR_LEDGE_BIT = 3;
const TUint32 ATTR_LEDGE = (ATTR_LEDGE_BIT << 16);



class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTilesSlot, TUint16 aMapSlot, TUint16 aCodesSlot);

  ~GGamePlayfield();
public:
  TBool IsWall(TFloat aWorldX, TFloat aWorldY) {
    return GetCell(aWorldX, aWorldY) & ATTR_WALL;
  }
};


#endif //MODITE_GGAMEPLAYFIELD_H
