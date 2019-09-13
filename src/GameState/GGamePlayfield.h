#ifndef MODITE_GGAMEPLAYFIELD_H
#define MODITE_GGAMEPLAYFIELD_H

#include "Game.h"

// these match the codes set for tiles in Pro Motion:
const TUint16 ATTR_FLOOR = 0;
const TUint16 ATTR_WALL = 1;
const TUint16 FLOOR_VOID_LEDGE = 3;

const TUint16 ATTR_PLAYER = 16;
const TUint16 ATTR_SPIDER = 17;
const TUint16 ATTR_BAT = 18;
const TUint16 ATTR_GOBLIN = 19;
const TUint16 ATTR_GOBLIN_SNIPER = 20;
const TUint16 ATTR_ORC = 21;
const TUint16 ATTR_RAT = 22;
const TUint16 ATTR_SLIME = 23;
const TUint16 ATTR_TROLL = 24;
const TUint16 ATTR_PROJECTILE_ARROW = 25;  // not sure this is going to be used like a GEnemyProcess

const TUint16 ATTR_SPIKES = 200;

const TUint16 ATTR_WALL_TORCH_LIT = 201;
const TUint16 ATTR_WALL_TORCH_UNLIT = 202;

const TUint16 ATTR_TREASURE_CHEST_CLOSED = 210;
const TUint16 ATTR_TREASURE_CHEST_OPENED = 211;

const TUint16 ATTR_STEP_SWITCH_UP = 303;
const TUint16 ATTR_STEP_SWITCH_DOWN = 304;

const TUint16 ATTR_FLOOR_SWITCH_LEFT = 305;
const TUint16 ATTR_FLOOR_SWITCH_MIDDLE = 306;
const TUint16 ATTR_FLOOR_SWITCH_RIGHT = 307;

const TUint16 ATTR_DOOR_SIDE_CLOSED = 310;
const TUint16 ATTR_DOOR_SIDE_OPEN = 311;
const TUint16 ATTR_DOOR_FRONT_CLOSED = 312;
const TUint16 ATTR_DOOR_FRONT_OPEN = 313;



const TUint16 ATTR_STAIRS_DOWN_EXIT = 500;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_1 = 501;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_2 = 502;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_3 = 503;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_4 = 504;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_5 = 505;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_6 = 506;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_7 = 507;
const TUint16 ATTR_STAIRS_DOWN_LEVEL_8 = 508;



const TUint16 ATTR_STAIRS_UP_EXIT = 600;
const TUint16 ATTR_STAIRS_UP_LEVEL_1 = 601;
const TUint16 ATTR_STAIRS_UP_LEVEL_2 = 602;
const TUint16 ATTR_STAIRS_UP_LEVEL_3 = 603;
const TUint16 ATTR_STAIRS_UP_LEVEL_4 = 604;
const TUint16 ATTR_STAIRS_UP_LEVEL_5 = 605;
const TUint16 ATTR_STAIRS_UP_LEVEL_6 = 606;
const TUint16 ATTR_STAIRS_UP_LEVEL_7 = 607;
const TUint16 ATTR_STAIRS_UP_LEVEL_8 = 608;




const TUint16 ATTR_LEVEL_0 = 501;
const TUint16 ATTR_LEVEL_1 = 510;
const TUint16 ATTR_LEVEL_2 = 510;
const TUint16 ATTR_LEVEL_3 = 510;
const TUint16 ATTR_LEVEL_4 = 510;
const TUint16 ATTR_LEVEL_5 = 510;
const TUint16 ATTR_LEVEL_6 = 516;
const TUint16 ATTR_LEVEL_7 = 517;






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
