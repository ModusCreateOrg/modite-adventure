#ifndef DUNGEON_DEFS_H
#define DUNGEON_DEFS_H
#include "Resources.h"

struct TDungeonInfo gDungeonDefs[] = {

  
  // OVERWORLD
  { 
    "Overworld",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OVERWORLD_OVERWORLD_L1_MAP, // 0 (Level 1 catch all)
        OVERWORLD_OVERWORLD_L1_MAP, // 1 woods
        OVERWORLD_OVERWORLD_L2_MAP, // 2 dark_dimension
        OVERWORLD_OVERWORLD_L3_MAP, // 3 ice_land
        OVERWORLD_OVERWORLD_L4_MAP, // 4 jungle
        OVERWORLD_OVERWORLD_L5_MAP, // 5 beach
        OVERWORLD_OVERWORLD_L6_MAP, // 6 jungle
        OVERWORLD_OVERWORLD_L7_MAP, // 7 ice_land
        OVERWORLD_OVERWORLD_L8_MAP, // 8 jungle
        OVERWORLD_OVERWORLD_L9_MAP, // 9 ashland
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },

};
#endif