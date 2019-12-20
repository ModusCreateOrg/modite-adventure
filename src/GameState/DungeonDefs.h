#ifndef DUNGEON_DEFS_H
#define DUNGEON_DEFS_H
#include "Resources.h"

struct TDungeonInfo gDungeonDefs[] = {
    { "OVERWORLD",
      {
        GLOBAL_OBJECT_LAYER_BMP,
        {
          OVERWORLD_OVERWORLD_L1_MAP, // 0
          OVERWORLD_OVERWORLD_L1_MAP, // 1
          OVERWORLD_OVERWORLD_L2_MAP, // 2
          OVERWORLD_OVERWORLD_L3_MAP, // 3
          OVERWORLD_OVERWORLD_L4_MAP, // 4
          OVERWORLD_OVERWORLD_L5_MAP, // 5
          OVERWORLD_OVERWORLD_L6_MAP, // 6
          OVERWORLD_OVERWORLD_L7_MAP, // 7
          OVERWORLD_OVERWORLD_L8_MAP, // 8
          OVERWORLD_OVERWORLD_L9_MAP, // 9
          -1                 // EMPTY -- no level 10 , evar!!
        },
      }
    },
//  {
//    "OW Beach",
//    {
//      GLOBAL_OBJECT_LAYER_BMP,
//      {
//          OVERWORLD_BEACH_OVERWORLD_BEACH_L1_MAP, // 0
//          OVERWORLD_BEACH_OVERWORLD_BEACH_L1_MAP, // 1
//          OVERWORLD_BEACH_OVERWORLD_BEACH_L2_MAP, // 2
//          OVERWORLD_BEACH_OVERWORLD_BEACH_L3_MAP, // 3
//          OVERWORLD_BEACH_OVERWORLD_BEACH_L4_MAP, // 4
//          -1, // 5
//          -1,               // 6
//          -1,               // 7
//          -1,               // 8
//          -1,               // 9
//          -1,               // 10
//      }
//    }
//  },
    {
      "DGN1",
        {
            GLOBAL_OBJECT_LAYER_BMP,
            {
                DGN1_DGN1_L1_MAP, // 0
                DGN1_DGN1_L1_MAP, // 1
                DGN1_DGN1_L2_MAP, // 2
                DGN1_DGN1_L3_MAP, // 3
                DGN1_DGN1_L4_MAP, // 4
                -1, // 5
                -1,               // 6
                -1,               // 7
                -1,               // 8
                -1,               // 9
                -1,               // 10
            }
        }
    },
    {
      "DGN2",
        {
            GLOBAL_OBJECT_LAYER_BMP,
            {
                DGN2_DGN2_L1_MAP, // 0
                DGN2_DGN2_L1_MAP, // 1
                DGN2_DGN2_L2_MAP, // 2
                DGN2_DGN2_L3_MAP, // 3
                DGN2_DGN2_L4_MAP, // 4
                -1, // 5
                -1,               // 6
                -1,               // 7
                -1,               // 8
                -1,               // 9
                -1,               // 10
            }
        }
    },
    {
      "DGN3",
        {
            GLOBAL_OBJECT_LAYER_BMP,
            {
                DGN3_DGN3_L1_MAP, // 0
                DGN3_DGN3_L1_MAP, // 1
                DGN3_DGN3_L2_MAP, // 2
                DGN3_DGN3_L3_MAP, // 3
                DGN3_DGN3_L4_MAP, // 4
                -1, // 5
                -1,               // 6
                -1,               // 7
                -1,               // 8
                -1,               // 9
                -1,               // 10
            }
        }
    },

};


#endif