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
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 1 Dungeon 1. This is the hero's home.
  { 
    "OW1_DGN1_256",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN1_256_OW1_DGN1_L1_MAP, // 0
        OW1_DGN1_256_OW1_DGN1_L1_MAP, // 1
        OW1_DGN1_256_OW1_DGN1_L2_MAP, // 2
        OW1_DGN1_256_OW1_DGN1_L3_MAP, // 3
        -1, // 4
        -1, // 5
        -1, // 6
        -1, // 7
        -1, // 8
        -1, // 9 
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 1 Dungeon 2. 
  { 
    "OW1_DGN2_512",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN2_512_OW1_DGN2_L1_MAP, // 0
        OW1_DGN2_512_OW1_DGN2_L1_MAP, // 1
        OW1_DGN2_512_OW1_DGN2_L2_MAP, // 2
        OW1_DGN2_512_OW1_DGN2_L3_MAP, // 3
        -1, // 4
        -1, // 5
        -1, // 6
        -1, // 7
        -1, // 8
        -1, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 1 Dungeon 3. 
  { 
    "OW1_DGN3_768",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN3_768_OW1_DGN3_L1_MAP, // 0
        OW1_DGN3_768_OW1_DGN3_L1_MAP, // 1
        OW1_DGN3_768_OW1_DGN3_L2_MAP, // 2
        OW1_DGN3_768_OW1_DGN3_L3_MAP, // 3
        OW1_DGN3_768_OW1_DGN3_L4_MAP, // 4
        OW1_DGN3_768_OW1_DGN3_L5_MAP, // 5
        OW1_DGN3_768_OW1_DGN3_L6_MAP, // 6
        OW1_DGN3_768_OW1_DGN3_L7_MAP, // 7
        OW1_DGN3_768_OW1_DGN3_L8_MAP, // 8
        OW1_DGN3_768_OW1_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 1 Dungeon 4. 
  { 
    "OW1_DGN4_1024",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN4_1024_OW1_DGN4_L1_MAP, // 0
        OW1_DGN4_1024_OW1_DGN4_L1_MAP, // 1
        OW1_DGN4_1024_OW1_DGN4_L2_MAP, // 2
        OW1_DGN4_1024_OW1_DGN4_L3_MAP, // 3
        OW1_DGN4_1024_OW1_DGN4_L4_MAP, // 4
        OW1_DGN4_1024_OW1_DGN4_L5_MAP, // 5
        OW1_DGN4_1024_OW1_DGN4_L6_MAP, // 6
        OW1_DGN4_1024_OW1_DGN4_L7_MAP, // 7
        OW1_DGN4_1024_OW1_DGN4_L8_MAP, // 8
        OW1_DGN4_1024_OW1_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 2 Dungeon 1. 
  { 
    "OW2_DGN1_1280",
    {
      OW2_DGN_OBJECT_LAYER_BMP,
      {
        OW2_DGN1_1280_OW2_DGN1_L1_MAP, // 0
        OW2_DGN1_1280_OW2_DGN1_L1_MAP, // 1
        OW2_DGN1_1280_OW2_DGN1_L2_MAP, // 2
        OW2_DGN1_1280_OW2_DGN1_L3_MAP, // 3
        OW2_DGN1_1280_OW2_DGN1_L4_MAP, // 4
        OW2_DGN1_1280_OW2_DGN1_L5_MAP, // 5
        OW2_DGN1_1280_OW2_DGN1_L6_MAP, // 6
        OW2_DGN1_1280_OW2_DGN1_L7_MAP, // 7
        OW2_DGN1_1280_OW2_DGN1_L8_MAP, // 8
        OW2_DGN1_1280_OW2_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 2 Dungeon 2. 
  { 
    "OW2_DGN2_1536",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN2_1536_OW2_DGN2_L1_MAP, // 0
        OW2_DGN2_1536_OW2_DGN2_L1_MAP, // 1
        OW2_DGN2_1536_OW2_DGN2_L2_MAP, // 2
        OW2_DGN2_1536_OW2_DGN2_L3_MAP, // 3
        OW2_DGN2_1536_OW2_DGN2_L4_MAP, // 4
        OW2_DGN2_1536_OW2_DGN2_L5_MAP, // 5
        OW2_DGN2_1536_OW2_DGN2_L6_MAP, // 6
        OW2_DGN2_1536_OW2_DGN2_L7_MAP, // 7
        OW2_DGN2_1536_OW2_DGN2_L8_MAP, // 8
        OW2_DGN2_1536_OW2_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 2 Dungeon 3. 
  { 
    "OW2_DGN3_1792",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN3_1792_OW2_DGN3_L1_MAP, // 0
        OW2_DGN3_1792_OW2_DGN3_L1_MAP, // 1
        OW2_DGN3_1792_OW2_DGN3_L2_MAP, // 2
        OW2_DGN3_1792_OW2_DGN3_L3_MAP, // 3
        OW2_DGN3_1792_OW2_DGN3_L4_MAP, // 4
        OW2_DGN3_1792_OW2_DGN3_L5_MAP, // 5
        OW2_DGN3_1792_OW2_DGN3_L6_MAP, // 6
        OW2_DGN3_1792_OW2_DGN3_L7_MAP, // 7
        OW2_DGN3_1792_OW2_DGN3_L8_MAP, // 8
        OW2_DGN3_1792_OW2_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 2 Dungeon 4. 
  { 
    "OW2_DGN4_2048",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN4_2048_OW2_DGN4_L1_MAP, // 0
        OW2_DGN4_2048_OW2_DGN4_L1_MAP, // 1
        OW2_DGN4_2048_OW2_DGN4_L2_MAP, // 2
        OW2_DGN4_2048_OW2_DGN4_L3_MAP, // 3
        OW2_DGN4_2048_OW2_DGN4_L4_MAP, // 4
        OW2_DGN4_2048_OW2_DGN4_L5_MAP, // 5
        OW2_DGN4_2048_OW2_DGN4_L6_MAP, // 6
        OW2_DGN4_2048_OW2_DGN4_L7_MAP, // 7
        OW2_DGN4_2048_OW2_DGN4_L8_MAP, // 8
        OW2_DGN4_2048_OW2_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 3 Dungeon 1. 
  { 
    "OW3_DGN1_2304",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN1_2304_OW3_DGN1_L1_MAP, // 0
        OW3_DGN1_2304_OW3_DGN1_L1_MAP, // 1
        OW3_DGN1_2304_OW3_DGN1_L2_MAP, // 2
        OW3_DGN1_2304_OW3_DGN1_L3_MAP, // 3
        OW3_DGN1_2304_OW3_DGN1_L4_MAP, // 4
        OW3_DGN1_2304_OW3_DGN1_L5_MAP, // 5
        OW3_DGN1_2304_OW3_DGN1_L6_MAP, // 6
        OW3_DGN1_2304_OW3_DGN1_L7_MAP, // 7
        OW3_DGN1_2304_OW3_DGN1_L8_MAP, // 8
        OW3_DGN1_2304_OW3_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 3 Dungeon 2. 
  { 
    "OW3_DGN2_2560",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN2_2560_OW3_DGN2_L1_MAP, // 0
        OW3_DGN2_2560_OW3_DGN2_L1_MAP, // 1
        OW3_DGN2_2560_OW3_DGN2_L2_MAP, // 2
        OW3_DGN2_2560_OW3_DGN2_L3_MAP, // 3
        OW3_DGN2_2560_OW3_DGN2_L4_MAP, // 4
        OW3_DGN2_2560_OW3_DGN2_L5_MAP, // 5
        OW3_DGN2_2560_OW3_DGN2_L6_MAP, // 6
        OW3_DGN2_2560_OW3_DGN2_L7_MAP, // 7
        OW3_DGN2_2560_OW3_DGN2_L8_MAP, // 8
        OW3_DGN2_2560_OW3_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 3 Dungeon 3. 
  { 
    "OW3_DGN3_2816",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN3_2816_OW3_DGN3_L1_MAP, // 0
        OW3_DGN3_2816_OW3_DGN3_L1_MAP, // 1
        OW3_DGN3_2816_OW3_DGN3_L2_MAP, // 2
        OW3_DGN3_2816_OW3_DGN3_L3_MAP, // 3
        OW3_DGN3_2816_OW3_DGN3_L4_MAP, // 4
        OW3_DGN3_2816_OW3_DGN3_L5_MAP, // 5
        OW3_DGN3_2816_OW3_DGN3_L6_MAP, // 6
        OW3_DGN3_2816_OW3_DGN3_L7_MAP, // 7
        OW3_DGN3_2816_OW3_DGN3_L8_MAP, // 8
        OW3_DGN3_2816_OW3_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 3 Dungeon 4. 
  { 
    "OW3_DGN4_3072",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN4_3072_OW3_DGN4_L1_MAP, // 0
        OW3_DGN4_3072_OW3_DGN4_L1_MAP, // 1
        OW3_DGN4_3072_OW3_DGN4_L2_MAP, // 2
        OW3_DGN4_3072_OW3_DGN4_L3_MAP, // 3
        OW3_DGN4_3072_OW3_DGN4_L4_MAP, // 4
        OW3_DGN4_3072_OW3_DGN4_L5_MAP, // 5
        OW3_DGN4_3072_OW3_DGN4_L6_MAP, // 6
        OW3_DGN4_3072_OW3_DGN4_L7_MAP, // 7
        OW3_DGN4_3072_OW3_DGN4_L8_MAP, // 8
        OW3_DGN4_3072_OW3_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 4 Dungeon 1. 
  { 
    "OW4_DGN1_3328",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN1_3328_OW4_DGN1_L1_MAP, // 0
        OW4_DGN1_3328_OW4_DGN1_L1_MAP, // 1
        OW4_DGN1_3328_OW4_DGN1_L2_MAP, // 2
        OW4_DGN1_3328_OW4_DGN1_L3_MAP, // 3
        OW4_DGN1_3328_OW4_DGN1_L4_MAP, // 4
        OW4_DGN1_3328_OW4_DGN1_L5_MAP, // 5
        OW4_DGN1_3328_OW4_DGN1_L6_MAP, // 6
        OW4_DGN1_3328_OW4_DGN1_L7_MAP, // 7
        OW4_DGN1_3328_OW4_DGN1_L8_MAP, // 8
        OW4_DGN1_3328_OW4_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 4 Dungeon 2. 
  { 
    "OW4_DGN2_3584",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN2_3584_OW4_DGN2_L1_MAP, // 0
        OW4_DGN2_3584_OW4_DGN2_L1_MAP, // 1
        OW4_DGN2_3584_OW4_DGN2_L2_MAP, // 2
        OW4_DGN2_3584_OW4_DGN2_L3_MAP, // 3
        OW4_DGN2_3584_OW4_DGN2_L4_MAP, // 4
        OW4_DGN2_3584_OW4_DGN2_L5_MAP, // 5
        OW4_DGN2_3584_OW4_DGN2_L6_MAP, // 6
        OW4_DGN2_3584_OW4_DGN2_L7_MAP, // 7
        OW4_DGN2_3584_OW4_DGN2_L8_MAP, // 8
        OW4_DGN2_3584_OW4_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 4 Dungeon 3. 
  { 
    "OW4_DGN3_3840",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN3_3840_OW4_DGN3_L1_MAP, // 0
        OW4_DGN3_3840_OW4_DGN3_L1_MAP, // 1
        OW4_DGN3_3840_OW4_DGN3_L2_MAP, // 2
        OW4_DGN3_3840_OW4_DGN3_L3_MAP, // 3
        OW4_DGN3_3840_OW4_DGN3_L4_MAP, // 4
        OW4_DGN3_3840_OW4_DGN3_L5_MAP, // 5
        OW4_DGN3_3840_OW4_DGN3_L6_MAP, // 6
        OW4_DGN3_3840_OW4_DGN3_L7_MAP, // 7
        OW4_DGN3_3840_OW4_DGN3_L8_MAP, // 8
        OW4_DGN3_3840_OW4_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 4 Dungeon 4. 
  { 
    "OW4_DGN4_4096",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN4_4096_OW4_DGN4_L1_MAP, // 0
        OW4_DGN4_4096_OW4_DGN4_L1_MAP, // 1
        OW4_DGN4_4096_OW4_DGN4_L2_MAP, // 2
        OW4_DGN4_4096_OW4_DGN4_L3_MAP, // 3
        OW4_DGN4_4096_OW4_DGN4_L4_MAP, // 4
        OW4_DGN4_4096_OW4_DGN4_L5_MAP, // 5
        OW4_DGN4_4096_OW4_DGN4_L6_MAP, // 6
        OW4_DGN4_4096_OW4_DGN4_L7_MAP, // 7
        OW4_DGN4_4096_OW4_DGN4_L8_MAP, // 8
        OW4_DGN4_4096_OW4_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 5 Dungeon 1. 
  { 
    "OW5_DGN1_4352",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN1_4352_OW5_DGN1_L1_MAP, // 0
        OW5_DGN1_4352_OW5_DGN1_L1_MAP, // 1
        OW5_DGN1_4352_OW5_DGN1_L2_MAP, // 2
        OW5_DGN1_4352_OW5_DGN1_L3_MAP, // 3
        OW5_DGN1_4352_OW5_DGN1_L4_MAP, // 4
        OW5_DGN1_4352_OW5_DGN1_L5_MAP, // 5
        OW5_DGN1_4352_OW5_DGN1_L6_MAP, // 6
        OW5_DGN1_4352_OW5_DGN1_L7_MAP, // 7
        OW5_DGN1_4352_OW5_DGN1_L8_MAP, // 8
        OW5_DGN1_4352_OW5_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 5 Dungeon 2. 
  { 
    "OW5_DGN2_4608",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN2_4608_OW5_DGN2_L1_MAP, // 0
        OW5_DGN2_4608_OW5_DGN2_L1_MAP, // 1
        OW5_DGN2_4608_OW5_DGN2_L2_MAP, // 2
        OW5_DGN2_4608_OW5_DGN2_L3_MAP, // 3
        OW5_DGN2_4608_OW5_DGN2_L4_MAP, // 4
        OW5_DGN2_4608_OW5_DGN2_L5_MAP, // 5
        OW5_DGN2_4608_OW5_DGN2_L6_MAP, // 6
        OW5_DGN2_4608_OW5_DGN2_L7_MAP, // 7
        OW5_DGN2_4608_OW5_DGN2_L8_MAP, // 8
        OW5_DGN2_4608_OW5_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 5 Dungeon 3. 
  { 
    "OW5_DGN3_4864",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN3_4864_OW5_DGN3_L1_MAP, // 0
        OW5_DGN3_4864_OW5_DGN3_L1_MAP, // 1
        OW5_DGN3_4864_OW5_DGN3_L2_MAP, // 2
        OW5_DGN3_4864_OW5_DGN3_L3_MAP, // 3
        OW5_DGN3_4864_OW5_DGN3_L4_MAP, // 4
        OW5_DGN3_4864_OW5_DGN3_L5_MAP, // 5
        OW5_DGN3_4864_OW5_DGN3_L6_MAP, // 6
        OW5_DGN3_4864_OW5_DGN3_L7_MAP, // 7
        OW5_DGN3_4864_OW5_DGN3_L8_MAP, // 8
        OW5_DGN3_4864_OW5_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 5 Dungeon 4. 
  { 
    "OW5_DGN4_5120",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN4_5120_OW5_DGN4_L1_MAP, // 0
        OW5_DGN4_5120_OW5_DGN4_L1_MAP, // 1
        OW5_DGN4_5120_OW5_DGN4_L2_MAP, // 2
        OW5_DGN4_5120_OW5_DGN4_L3_MAP, // 3
        OW5_DGN4_5120_OW5_DGN4_L4_MAP, // 4
        OW5_DGN4_5120_OW5_DGN4_L5_MAP, // 5
        OW5_DGN4_5120_OW5_DGN4_L6_MAP, // 6
        OW5_DGN4_5120_OW5_DGN4_L7_MAP, // 7
        OW5_DGN4_5120_OW5_DGN4_L8_MAP, // 8
        OW5_DGN4_5120_OW5_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 6 Dungeon 1. 
  { 
    "OW6_DGN1_5376",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN1_5376_OW6_DGN1_L1_MAP, // 0
        OW6_DGN1_5376_OW6_DGN1_L1_MAP, // 1
        OW6_DGN1_5376_OW6_DGN1_L2_MAP, // 2
        OW6_DGN1_5376_OW6_DGN1_L3_MAP, // 3
        OW6_DGN1_5376_OW6_DGN1_L4_MAP, // 4
        OW6_DGN1_5376_OW6_DGN1_L5_MAP, // 5
        OW6_DGN1_5376_OW6_DGN1_L6_MAP, // 6
        OW6_DGN1_5376_OW6_DGN1_L7_MAP, // 7
        OW6_DGN1_5376_OW6_DGN1_L8_MAP, // 8
        OW6_DGN1_5376_OW6_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 6 Dungeon 2. 
  { 
    "OW6_DGN2_5632",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN2_5632_OW6_DGN2_L1_MAP, // 0
        OW6_DGN2_5632_OW6_DGN2_L1_MAP, // 1
        OW6_DGN2_5632_OW6_DGN2_L2_MAP, // 2
        OW6_DGN2_5632_OW6_DGN2_L3_MAP, // 3
        OW6_DGN2_5632_OW6_DGN2_L4_MAP, // 4
        OW6_DGN2_5632_OW6_DGN2_L5_MAP, // 5
        OW6_DGN2_5632_OW6_DGN2_L6_MAP, // 6
        OW6_DGN2_5632_OW6_DGN2_L7_MAP, // 7
        OW6_DGN2_5632_OW6_DGN2_L8_MAP, // 8
        OW6_DGN2_5632_OW6_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 6 Dungeon 3. 
  { 
    "OW6_DGN3_5888",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN3_5888_OW6_DGN3_L1_MAP, // 0
        OW6_DGN3_5888_OW6_DGN3_L1_MAP, // 1
        OW6_DGN3_5888_OW6_DGN3_L2_MAP, // 2
        OW6_DGN3_5888_OW6_DGN3_L3_MAP, // 3
        OW6_DGN3_5888_OW6_DGN3_L4_MAP, // 4
        OW6_DGN3_5888_OW6_DGN3_L5_MAP, // 5
        OW6_DGN3_5888_OW6_DGN3_L6_MAP, // 6
        OW6_DGN3_5888_OW6_DGN3_L7_MAP, // 7
        OW6_DGN3_5888_OW6_DGN3_L8_MAP, // 8
        OW6_DGN3_5888_OW6_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 6 Dungeon 4. 
  { 
    "OW6_DGN4_6144",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN4_6144_OW6_DGN4_L1_MAP, // 0
        OW6_DGN4_6144_OW6_DGN4_L1_MAP, // 1
        OW6_DGN4_6144_OW6_DGN4_L2_MAP, // 2
        OW6_DGN4_6144_OW6_DGN4_L3_MAP, // 3
        OW6_DGN4_6144_OW6_DGN4_L4_MAP, // 4
        OW6_DGN4_6144_OW6_DGN4_L5_MAP, // 5
        OW6_DGN4_6144_OW6_DGN4_L6_MAP, // 6
        OW6_DGN4_6144_OW6_DGN4_L7_MAP, // 7
        OW6_DGN4_6144_OW6_DGN4_L8_MAP, // 8
        OW6_DGN4_6144_OW6_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 7 Dungeon 1. 
  { 
    "OW7_DGN1_6400",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN1_6400_OW7_DGN1_L1_MAP, // 0
        OW7_DGN1_6400_OW7_DGN1_L1_MAP, // 1
        OW7_DGN1_6400_OW7_DGN1_L2_MAP, // 2
        OW7_DGN1_6400_OW7_DGN1_L3_MAP, // 3
        OW7_DGN1_6400_OW7_DGN1_L4_MAP, // 4
        OW7_DGN1_6400_OW7_DGN1_L5_MAP, // 5
        OW7_DGN1_6400_OW7_DGN1_L6_MAP, // 6
        OW7_DGN1_6400_OW7_DGN1_L7_MAP, // 7
        OW7_DGN1_6400_OW7_DGN1_L8_MAP, // 8
        OW7_DGN1_6400_OW7_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 7 Dungeon 2. 
  { 
    "OW7_DGN2_6656",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN2_6656_OW7_DGN2_L1_MAP, // 0
        OW7_DGN2_6656_OW7_DGN2_L1_MAP, // 1
        OW7_DGN2_6656_OW7_DGN2_L2_MAP, // 2
        OW7_DGN2_6656_OW7_DGN2_L3_MAP, // 3
        OW7_DGN2_6656_OW7_DGN2_L4_MAP, // 4
        OW7_DGN2_6656_OW7_DGN2_L5_MAP, // 5
        OW7_DGN2_6656_OW7_DGN2_L6_MAP, // 6
        OW7_DGN2_6656_OW7_DGN2_L7_MAP, // 7
        OW7_DGN2_6656_OW7_DGN2_L8_MAP, // 8
        OW7_DGN2_6656_OW7_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 7 Dungeon 3. 
  { 
    "OW7_DGN3_6912",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN3_6912_OW7_DGN3_L1_MAP, // 0
        OW7_DGN3_6912_OW7_DGN3_L1_MAP, // 1
        OW7_DGN3_6912_OW7_DGN3_L2_MAP, // 2
        OW7_DGN3_6912_OW7_DGN3_L3_MAP, // 3
        OW7_DGN3_6912_OW7_DGN3_L4_MAP, // 4
        OW7_DGN3_6912_OW7_DGN3_L5_MAP, // 5
        OW7_DGN3_6912_OW7_DGN3_L6_MAP, // 6
        OW7_DGN3_6912_OW7_DGN3_L7_MAP, // 7
        OW7_DGN3_6912_OW7_DGN3_L8_MAP, // 8
        OW7_DGN3_6912_OW7_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 7 Dungeon 4. 
  { 
    "OW7_DGN4_7168",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN4_7168_OW7_DGN4_L1_MAP, // 0
        OW7_DGN4_7168_OW7_DGN4_L1_MAP, // 1
        OW7_DGN4_7168_OW7_DGN4_L2_MAP, // 2
        OW7_DGN4_7168_OW7_DGN4_L3_MAP, // 3
        OW7_DGN4_7168_OW7_DGN4_L4_MAP, // 4
        OW7_DGN4_7168_OW7_DGN4_L5_MAP, // 5
        OW7_DGN4_7168_OW7_DGN4_L6_MAP, // 6
        OW7_DGN4_7168_OW7_DGN4_L7_MAP, // 7
        OW7_DGN4_7168_OW7_DGN4_L8_MAP, // 8
        OW7_DGN4_7168_OW7_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 8 Dungeon 1. 
  { 
    "OW8_DGN1_7424",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN1_7424_OW8_DGN1_L1_MAP, // 0
        OW8_DGN1_7424_OW8_DGN1_L1_MAP, // 1
        OW8_DGN1_7424_OW8_DGN1_L2_MAP, // 2
        OW8_DGN1_7424_OW8_DGN1_L3_MAP, // 3
        OW8_DGN1_7424_OW8_DGN1_L4_MAP, // 4
        OW8_DGN1_7424_OW8_DGN1_L5_MAP, // 5
        OW8_DGN1_7424_OW8_DGN1_L6_MAP, // 6
        OW8_DGN1_7424_OW8_DGN1_L7_MAP, // 7
        OW8_DGN1_7424_OW8_DGN1_L8_MAP, // 8
        OW8_DGN1_7424_OW8_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 8 Dungeon 2. 
  { 
    "OW8_DGN2_7680",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN2_7680_OW8_DGN2_L1_MAP, // 0
        OW8_DGN2_7680_OW8_DGN2_L1_MAP, // 1
        OW8_DGN2_7680_OW8_DGN2_L2_MAP, // 2
        OW8_DGN2_7680_OW8_DGN2_L3_MAP, // 3
        OW8_DGN2_7680_OW8_DGN2_L4_MAP, // 4
        OW8_DGN2_7680_OW8_DGN2_L5_MAP, // 5
        OW8_DGN2_7680_OW8_DGN2_L6_MAP, // 6
        OW8_DGN2_7680_OW8_DGN2_L7_MAP, // 7
        OW8_DGN2_7680_OW8_DGN2_L8_MAP, // 8
        OW8_DGN2_7680_OW8_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 8 Dungeon 3. 
  { 
    "OW8_DGN3_7936",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN3_7936_OW8_DGN3_L1_MAP, // 0
        OW8_DGN3_7936_OW8_DGN3_L1_MAP, // 1
        OW8_DGN3_7936_OW8_DGN3_L2_MAP, // 2
        OW8_DGN3_7936_OW8_DGN3_L3_MAP, // 3
        OW8_DGN3_7936_OW8_DGN3_L4_MAP, // 4
        OW8_DGN3_7936_OW8_DGN3_L5_MAP, // 5
        OW8_DGN3_7936_OW8_DGN3_L6_MAP, // 6
        OW8_DGN3_7936_OW8_DGN3_L7_MAP, // 7
        OW8_DGN3_7936_OW8_DGN3_L8_MAP, // 8
        OW8_DGN3_7936_OW8_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 8 Dungeon 4. 
  { 
    "OW8_DGN4_8192",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN4_8192_OW8_DGN4_L1_MAP, // 0
        OW8_DGN4_8192_OW8_DGN4_L1_MAP, // 1
        OW8_DGN4_8192_OW8_DGN4_L2_MAP, // 2
        OW8_DGN4_8192_OW8_DGN4_L3_MAP, // 3
        OW8_DGN4_8192_OW8_DGN4_L4_MAP, // 4
        OW8_DGN4_8192_OW8_DGN4_L5_MAP, // 5
        OW8_DGN4_8192_OW8_DGN4_L6_MAP, // 6
        OW8_DGN4_8192_OW8_DGN4_L7_MAP, // 7
        OW8_DGN4_8192_OW8_DGN4_L8_MAP, // 8
        OW8_DGN4_8192_OW8_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 9 Dungeon 1. 
  { 
    "OW9_DGN1_8448",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN1_8448_OW9_DGN1_L1_MAP, // 0
        OW9_DGN1_8448_OW9_DGN1_L1_MAP, // 1
        OW9_DGN1_8448_OW9_DGN1_L2_MAP, // 2
        OW9_DGN1_8448_OW9_DGN1_L3_MAP, // 3
        OW9_DGN1_8448_OW9_DGN1_L4_MAP, // 4
        OW9_DGN1_8448_OW9_DGN1_L5_MAP, // 5
        OW9_DGN1_8448_OW9_DGN1_L6_MAP, // 6
        OW9_DGN1_8448_OW9_DGN1_L7_MAP, // 7
        OW9_DGN1_8448_OW9_DGN1_L8_MAP, // 8
        OW9_DGN1_8448_OW9_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 9 Dungeon 2. 
  { 
    "OW9_DGN2_8704",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN2_8704_OW9_DGN2_L1_MAP, // 0
        OW9_DGN2_8704_OW9_DGN2_L1_MAP, // 1
        OW9_DGN2_8704_OW9_DGN2_L2_MAP, // 2
        OW9_DGN2_8704_OW9_DGN2_L3_MAP, // 3
        OW9_DGN2_8704_OW9_DGN2_L4_MAP, // 4
        OW9_DGN2_8704_OW9_DGN2_L5_MAP, // 5
        OW9_DGN2_8704_OW9_DGN2_L6_MAP, // 6
        OW9_DGN2_8704_OW9_DGN2_L7_MAP, // 7
        OW9_DGN2_8704_OW9_DGN2_L8_MAP, // 8
        OW9_DGN2_8704_OW9_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 9 Dungeon 3. 
  { 
    "OW9_DGN3_8960",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN3_8960_OW9_DGN3_L1_MAP, // 0
        OW9_DGN3_8960_OW9_DGN3_L1_MAP, // 1
        OW9_DGN3_8960_OW9_DGN3_L2_MAP, // 2
        OW9_DGN3_8960_OW9_DGN3_L3_MAP, // 3
        OW9_DGN3_8960_OW9_DGN3_L4_MAP, // 4
        OW9_DGN3_8960_OW9_DGN3_L5_MAP, // 5
        OW9_DGN3_8960_OW9_DGN3_L6_MAP, // 6
        OW9_DGN3_8960_OW9_DGN3_L7_MAP, // 7
        OW9_DGN3_8960_OW9_DGN3_L8_MAP, // 8
        OW9_DGN3_8960_OW9_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVERWORLD 9 Dungeon 4. 
  { 
    "OW9_DGN4_9216",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN4_9216_OW9_DGN4_L1_MAP, // 0
        OW9_DGN4_9216_OW9_DGN4_L1_MAP, // 1
        OW9_DGN4_9216_OW9_DGN4_L2_MAP, // 2
        OW9_DGN4_9216_OW9_DGN4_L3_MAP, // 3
        OW9_DGN4_9216_OW9_DGN4_L4_MAP, // 4
        OW9_DGN4_9216_OW9_DGN4_L5_MAP, // 5
        OW9_DGN4_9216_OW9_DGN4_L6_MAP, // 6
        OW9_DGN4_9216_OW9_DGN4_L7_MAP, // 7
        OW9_DGN4_9216_OW9_DGN4_L8_MAP, // 8
        OW9_DGN4_9216_OW9_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  // OVERWORLD 9 Dungeon 5 (Also Dev Dungeon!!).
  {
    "OWX_DEVDGN_9472",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OWX_DEVDGN_9472_OWX_DEVDGN_L1_MAP, // 0
        OWX_DEVDGN_9472_OWX_DEVDGN_L1_MAP, // 1
        OWX_DEVDGN_9472_OWX_DEVDGN_L2_MAP, // 2
        OWX_DEVDGN_9472_OWX_DEVDGN_L3_MAP, // 3
        OWX_DEVDGN_9472_OWX_DEVDGN_L4_MAP, // 4
        OWX_DEVDGN_9472_OWX_DEVDGN_L5_MAP, // 5
        OWX_DEVDGN_9472_OWX_DEVDGN_L6_MAP, // 6
        OWX_DEVDGN_9472_OWX_DEVDGN_L7_MAP, // 7
        OWX_DEVDGN_9472_OWX_DEVDGN_L8_MAP, // 8
        OWX_DEVDGN_9472_OWX_DEVDGN_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  }
};
#endif