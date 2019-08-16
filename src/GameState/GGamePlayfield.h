#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

#include "Game.h"

// these match the codes set for tiles in Pro Motion:
const TUint16 ATTR_FLOOR = 0;
const TUint16 ATTR_WALL = 1;

const TUint16 ATTR_PLAYER = 16;
const TUint16 ATTR_SPIDER = 17;
const TUint16 ATTR_BAT = 18;
const TUint16 ATTR_GOBLIN = 19;
const TUint16 ATTR_GOBLIN_SNIPER = 20;
const TUint16 ATTR_ORC = 21;
const TUint16 ATTR_RAT = 22;
const TUint16 ATTR_SLIME = 23;
const TUint16 ATTR_TROLL = 24;
const TUint16 ATTR_PROJECTIL_ARROW = 25;  // not sure this is going to be used like a GEnemyProcess

class GGamePlayfield : public BMapPlayfield {
public:
  GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId);

  ~GGamePlayfield();
public:
  TBool IsWall(TFloat aWorldX, TFloat aWorldY) {
    return TUint16(GetCell(aWorldX, aWorldY)>>16) == ATTR_WALL;
  }
};


#endif //MODITE_GGAMEPLAYFIELD_H
