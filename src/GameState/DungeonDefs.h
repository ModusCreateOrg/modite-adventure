#ifndef DUNGEON_DEFS_H
#define DUNGEON_DEFS_H
#include "Resources.h"

struct TDungeonInfo gDungeonDefs[] = {

  
  // OVER WORLD
  { 
    "Over world",
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
  
  // OVER WORLD 1 Dungeon 1. This is the hero's home.
  { 
    "O1 D1",
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
  
  // OVER WORLD 1 Dungeon 2. 
  { 
    "O1 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN2_512_OW1_DGN2_L1_MAP, // 0
        OW1_DGN2_512_OW1_DGN2_L1_MAP, // 1
        OW1_DGN2_512_OW1_DGN2_L2_MAP, // 2
        OW1_DGN2_512_OW1_DGN2_L3_MAP, // 3
        OW1_DGN2_512_OW1_DGN2_L4_MAP, // 4
        -1, // 5
        -1, // 6
        -1, // 7
        -1, // 8
        -1, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 1 Dungeon 3. 
  { 
    "O1 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN3_768_OW1_DGN3_L1_MAP, // 0
        OW1_DGN3_768_OW1_DGN3_L1_MAP, // 1
        -1, //OW1_DGN3_768_OW1_DGN3_L2_MAP, // 2
        -1, //OW1_DGN3_768_OW1_DGN3_L3_MAP, // 3
        -1, //OW1_DGN3_768_OW1_DGN3_L4_MAP, // 4
        -1, //OW1_DGN3_768_OW1_DGN3_L5_MAP, // 5
        -1, //OW1_DGN3_768_OW1_DGN3_L6_MAP, // 6
        -1, //OW1_DGN3_768_OW1_DGN3_L7_MAP, // 7
        -1, //OW1_DGN3_768_OW1_DGN3_L8_MAP, // 8
        -1, //OW1_DGN3_768_OW1_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 1 Dungeon 4. 
  { 
    "O1 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW1_DGN4_1024_OW1_DGN4_L1_MAP, // 0
        OW1_DGN4_1024_OW1_DGN4_L1_MAP, // 1
        -1, // OW1_DGN4_1024_OW1_DGN4_L2_MAP, // 2
        -1, // OW1_DGN4_1024_OW1_DGN4_L3_MAP, // 3
        -1, // OW1_DGN4_1024_OW1_DGN4_L4_MAP, // 4
        -1, // OW1_DGN4_1024_OW1_DGN4_L5_MAP, // 5
        -1, // OW1_DGN4_1024_OW1_DGN4_L6_MAP, // 6
        -1, // OW1_DGN4_1024_OW1_DGN4_L7_MAP, // 7
        -1, //OW1_DGN4_1024_OW1_DGN4_L8_MAP, // 8
        -1, //OW1_DGN4_1024_OW1_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 2 Dungeon 1. 
  { 
    "O2 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN1_1280_OW2_DGN1_L1_MAP, // 0
        OW2_DGN1_1280_OW2_DGN1_L1_MAP, // 1
        OW2_DGN1_1280_OW2_DGN1_L2_MAP, // 2
        OW2_DGN1_1280_OW2_DGN1_L3_MAP, // 3
        -1, // OW2_DGN1_1280_OW2_DGN1_L4_MAP, // 4
        -1, // OW2_DGN1_1280_OW2_DGN1_L5_MAP, // 5
        -1, // OW2_DGN1_1280_OW2_DGN1_L6_MAP, // 6
        -1, // OW2_DGN1_1280_OW2_DGN1_L7_MAP, // 7
        -1, // OW2_DGN1_1280_OW2_DGN1_L8_MAP, // 8
        -1, // OW2_DGN1_1280_OW2_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 2 Dungeon 2. 
  { 
    "O2 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN2_1536_OW2_DGN2_L1_MAP, // 0
        OW2_DGN2_1536_OW2_DGN2_L1_MAP, // 1
        OW2_DGN2_1536_OW2_DGN2_L2_MAP, // 2
        OW2_DGN2_1536_OW2_DGN2_L3_MAP, // 3
        OW2_DGN2_1536_OW2_DGN2_L4_MAP, // 4
        -1, //OW2_DGN2_1536_OW2_DGN2_L5_MAP, // 5
        -1, //OW2_DGN2_1536_OW2_DGN2_L6_MAP, // 6
        -1, //OW2_DGN2_1536_OW2_DGN2_L7_MAP, // 7
        -1, //OW2_DGN2_1536_OW2_DGN2_L8_MAP, // 8
        -1, //OW2_DGN2_1536_OW2_DGN2_L9_MAP, // 9
        -1,  // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 2 Dungeon 3. 
  { 
    "O2 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN3_1792_OW2_DGN3_L1_MAP, // 0
        OW2_DGN3_1792_OW2_DGN3_L1_MAP, // 1
        OW2_DGN3_1792_OW2_DGN3_L2_MAP, // 2
        OW2_DGN3_1792_OW2_DGN3_L3_MAP, // 3
        OW2_DGN3_1792_OW2_DGN3_L4_MAP, // 4
        -1, // OW2_DGN3_1792_OW2_DGN3_L5_MAP, // 5
        -1, // OW2_DGN3_1792_OW2_DGN3_L6_MAP, // 6
        -1, // OW2_DGN3_1792_OW2_DGN3_L7_MAP, // 7
        -1, // OW2_DGN3_1792_OW2_DGN3_L8_MAP, // 8
        -1, // OW2_DGN3_1792_OW2_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 2 Dungeon 4. 
  { 
    "O2 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN4_2048_OW2_DGN4_L1_MAP, // 0
        OW2_DGN4_2048_OW2_DGN4_L1_MAP, // 1
        OW2_DGN4_2048_OW2_DGN4_L2_MAP, // 2
        OW2_DGN4_2048_OW2_DGN4_L3_MAP, // 3
        OW2_DGN4_2048_OW2_DGN4_L4_MAP, // 4
        OW2_DGN4_2048_OW2_DGN4_L5_MAP, // 5
        -1, //OW2_DGN4_2048_OW2_DGN4_L6_MAP, // 6
        -1, //OW2_DGN4_2048_OW2_DGN4_L7_MAP, // 7
        -1, //OW2_DGN4_2048_OW2_DGN4_L8_MAP, // 8
        -1, // OW2_DGN4_2048_OW2_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 3 Dungeon 1. 
  { 
    "O3 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN1_2304_OW3_DGN1_L1_MAP, // 0
        OW3_DGN1_2304_OW3_DGN1_L1_MAP, // 1
        OW3_DGN1_2304_OW3_DGN1_L2_MAP, // 2
        -1, //OW3_DGN1_2304_OW3_DGN1_L3_MAP, // 3
        -1, //OW3_DGN1_2304_OW3_DGN1_L4_MAP, // 4
        -1, //OW3_DGN1_2304_OW3_DGN1_L5_MAP, // 5
        -1, //OW3_DGN1_2304_OW3_DGN1_L6_MAP, // 6
        -1, //OW3_DGN1_2304_OW3_DGN1_L7_MAP, // 7
        -1, //OW3_DGN1_2304_OW3_DGN1_L8_MAP, // 8
        -1, //OW3_DGN1_2304_OW3_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 3 Dungeon 2. 
  { 
    "O3 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN2_2560_OW3_DGN2_L1_MAP, // 0
        OW3_DGN2_2560_OW3_DGN2_L1_MAP, // 1
        OW3_DGN2_2560_OW3_DGN2_L2_MAP, // 2
        OW3_DGN2_2560_OW3_DGN2_L3_MAP, // 3
        -1, //OW3_DGN2_2560_OW3_DGN2_L4_MAP, // 4
        -1, //OW3_DGN2_2560_OW3_DGN2_L5_MAP, // 5
        -1, //OW3_DGN2_2560_OW3_DGN2_L6_MAP, // 6
        -1, //OW3_DGN2_2560_OW3_DGN2_L7_MAP, // 7
        -1, //OW3_DGN2_2560_OW3_DGN2_L8_MAP, // 8
        -1, //OW3_DGN2_2560_OW3_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 3 Dungeon 3. 
  { 
    "O3 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW3_DGN3_2816_OW3_DGN3_L1_MAP, // 0
        OW3_DGN3_2816_OW3_DGN3_L1_MAP, // 1
        -1, //OW3_DGN3_2816_OW3_DGN3_L2_MAP, // 2
        -1, //OW3_DGN3_2816_OW3_DGN3_L3_MAP, // 3
        -1, //OW3_DGN3_2816_OW3_DGN3_L4_MAP, // 4
        -1, //OW3_DGN3_2816_OW3_DGN3_L5_MAP, // 5
        -1, //OW3_DGN3_2816_OW3_DGN3_L6_MAP, // 6
        -1, //OW3_DGN3_2816_OW3_DGN3_L7_MAP, // 7
        -1, //OW3_DGN3_2816_OW3_DGN3_L8_MAP, // 8
        -1, //OW3_DGN3_2816_OW3_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 3 Dungeon 4. 
  { 
    "O3 D4",
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
        -1, //OW3_DGN4_3072_OW3_DGN4_L7_MAP, // 7
        -1, //OW3_DGN4_3072_OW3_DGN4_L8_MAP, // 8
        -1, // OW3_DGN4_3072_OW3_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 4 Dungeon 1. 
  { 
    "O4 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN1_3328_OW4_DGN1_L1_MAP, // 0
        OW4_DGN1_3328_OW4_DGN1_L1_MAP, // 1
        -1, // OW4_DGN1_3328_OW4_DGN1_L2_MAP, // 2
        -1, // OW4_DGN1_3328_OW4_DGN1_L3_MAP, // 3
        -1, // OW4_DGN1_3328_OW4_DGN1_L4_MAP, // 4
        -1, // OW4_DGN1_3328_OW4_DGN1_L5_MAP, // 5
        -1, // OW4_DGN1_3328_OW4_DGN1_L6_MAP, // 6
        -1, // OW4_DGN1_3328_OW4_DGN1_L7_MAP, // 7
        -1, // OW4_DGN1_3328_OW4_DGN1_L8_MAP, // 8
        -1, // OW4_DGN1_3328_OW4_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 4 Dungeon 2. 
  { 
    "O4 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN2_3584_OW4_DGN2_L1_MAP, // 0
        OW4_DGN2_3584_OW4_DGN2_L1_MAP, // 1
        OW4_DGN2_3584_OW4_DGN2_L2_MAP, // 2
        OW4_DGN2_3584_OW4_DGN2_L3_MAP, // 3
        OW4_DGN2_3584_OW4_DGN2_L4_MAP, // 4
        -1, //OW4_DGN2_3584_OW4_DGN2_L5_MAP, // 5
        -1, // OW4_DGN2_3584_OW4_DGN2_L6_MAP, // 6
        -1, // OW4_DGN2_3584_OW4_DGN2_L7_MAP, // 7
        -1, // OW4_DGN2_3584_OW4_DGN2_L8_MAP, // 8
        -1, //OW4_DGN2_3584_OW4_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 4 Dungeon 3. 
  { 
    "O4 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN3_3840_OW4_DGN3_L1_MAP, // 0
        OW4_DGN3_3840_OW4_DGN3_L1_MAP, // 1
        -1, //OW4_DGN3_3840_OW4_DGN3_L2_MAP, // 2
        -1, //OW4_DGN3_3840_OW4_DGN3_L3_MAP, // 3
        -1, //OW4_DGN3_3840_OW4_DGN3_L4_MAP, // 4
        -1, //OW4_DGN3_3840_OW4_DGN3_L5_MAP, // 5
        -1, //OW4_DGN3_3840_OW4_DGN3_L6_MAP, // 6
        -1, //OW4_DGN3_3840_OW4_DGN3_L7_MAP, // 7
        -1, //OW4_DGN3_3840_OW4_DGN3_L8_MAP, // 8
         -1, // OW4_DGN3_3840_OW4_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 4 Dungeon 4. 
  { 
    "O4 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW4_DGN4_4096_OW4_DGN4_L1_MAP, // 0
        OW4_DGN4_4096_OW4_DGN4_L1_MAP, // 1
        OW4_DGN4_4096_OW4_DGN4_L2_MAP, // 2
        OW4_DGN4_4096_OW4_DGN4_L3_MAP, // 3
        OW4_DGN4_4096_OW4_DGN4_L4_MAP, // 4
        OW4_DGN4_4096_OW4_DGN4_L5_MAP, // 5
        -1, //OW4_DGN4_4096_OW4_DGN4_L6_MAP, // 6
        -1, //OW4_DGN4_4096_OW4_DGN4_L7_MAP, // 7
        -1, //OW4_DGN4_4096_OW4_DGN4_L8_MAP, // 8
        -1, //OW4_DGN4_4096_OW4_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 5 Dungeon 1. 
  { 
    "O5 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN1_4352_OW5_DGN1_L1_MAP, // 0
        OW5_DGN1_4352_OW5_DGN1_L1_MAP, // 1
        OW5_DGN1_4352_OW5_DGN1_L2_MAP, // 2
          -1, //OW5_DGN1_4352_OW5_DGN1_L3_MAP, // 3
          -1, //OW5_DGN1_4352_OW5_DGN1_L4_MAP, // 4
          -1, //OW5_DGN1_4352_OW5_DGN1_L5_MAP, // 5
          -1, //OW5_DGN1_4352_OW5_DGN1_L6_MAP, // 6
          -1, //OW5_DGN1_4352_OW5_DGN1_L7_MAP, // 7
          -1, //OW5_DGN1_4352_OW5_DGN1_L8_MAP, // 8
          -1, //OW5_DGN1_4352_OW5_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 5 Dungeon 2. 
  { 
    "O5 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN2_4608_OW5_DGN2_L1_MAP, // 0
        OW5_DGN2_4608_OW5_DGN2_L1_MAP, // 1
        -1, //OW5_DGN2_4608_OW5_DGN2_L2_MAP, // 2
        -1, //OW5_DGN2_4608_OW5_DGN2_L3_MAP, // 3
        -1, // OW5_DGN2_4608_OW5_DGN2_L4_MAP, // 4
        -1, // OW5_DGN2_4608_OW5_DGN2_L5_MAP, // 5
        -1, // OW5_DGN2_4608_OW5_DGN2_L6_MAP, // 6
        -1, // OW5_DGN2_4608_OW5_DGN2_L7_MAP, // 7
        -1, // OW5_DGN2_4608_OW5_DGN2_L8_MAP, // 8
        -1, // OW5_DGN2_4608_OW5_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 5 Dungeon 3. 
  { 
    "O5 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN3_4864_OW5_DGN3_L1_MAP, // 0
        OW5_DGN3_4864_OW5_DGN3_L1_MAP, // 1
        OW5_DGN3_4864_OW5_DGN3_L2_MAP, // 2
        OW5_DGN3_4864_OW5_DGN3_L3_MAP, // 3
        -1, //OW5_DGN3_4864_OW5_DGN3_L4_MAP, // 4
        -1, //OW5_DGN3_4864_OW5_DGN3_L5_MAP, // 5
        -1, //OW5_DGN3_4864_OW5_DGN3_L6_MAP, // 6
        -1, //OW5_DGN3_4864_OW5_DGN3_L7_MAP, // 7
        -1, //OW5_DGN3_4864_OW5_DGN3_L8_MAP, // 8
        -1, //OW5_DGN3_4864_OW5_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 5 Dungeon 4. 
  { 
    "O5 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW5_DGN4_5120_OW5_DGN4_L1_MAP, // 0
        OW5_DGN4_5120_OW5_DGN4_L1_MAP, // 1
        OW5_DGN4_5120_OW5_DGN4_L2_MAP, // 2
        OW5_DGN4_5120_OW5_DGN4_L3_MAP, // 3
        OW5_DGN4_5120_OW5_DGN4_L4_MAP, // 4
        OW5_DGN4_5120_OW5_DGN4_L5_MAP, // 5
        -1, //OW5_DGN4_5120_OW5_DGN4_L6_MAP, // 6
        -1, //OW5_DGN4_5120_OW5_DGN4_L7_MAP, // 7
        -1, // OW5_DGN4_5120_OW5_DGN4_L8_MAP, // 8
        -1, // OW5_DGN4_5120_OW5_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 6 Dungeon 1. 
  { 
    "O6 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN1_5376_OW6_DGN1_L1_MAP, // 0
        OW6_DGN1_5376_OW6_DGN1_L1_MAP, // 1
        OW6_DGN1_5376_OW6_DGN1_L2_MAP, // 2
        -1, //OW6_DGN1_5376_OW6_DGN1_L3_MAP, // 3
        -1, //OW6_DGN1_5376_OW6_DGN1_L4_MAP, // 4
        -1, //OW6_DGN1_5376_OW6_DGN1_L5_MAP, // 5
        -1, //OW6_DGN1_5376_OW6_DGN1_L6_MAP, // 6
        -1, // OW6_DGN1_5376_OW6_DGN1_L7_MAP, // 7
        -1, // OW6_DGN1_5376_OW6_DGN1_L8_MAP, // 8
        -1, // OW6_DGN1_5376_OW6_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 6 Dungeon 2. 
  { 
    "O6 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW6_DGN2_5632_OW6_DGN2_L1_MAP, // 0
        OW6_DGN2_5632_OW6_DGN2_L1_MAP, // 1
        -1, //OW6_DGN2_5632_OW6_DGN2_L2_MAP, // 2
        -1, // OW6_DGN2_5632_OW6_DGN2_L3_MAP, // 3
        -1, // OW6_DGN2_5632_OW6_DGN2_L4_MAP, // 4
        -1, // OW6_DGN2_5632_OW6_DGN2_L5_MAP, // 5
        -1, // OW6_DGN2_5632_OW6_DGN2_L6_MAP, // 6
        -1, //OW6_DGN2_5632_OW6_DGN2_L7_MAP, // 7
        -1, // OW6_DGN2_5632_OW6_DGN2_L8_MAP, // 8
        -1, //OW6_DGN2_5632_OW6_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 6 Dungeon 3. 
  { 
    "O6 D3",
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
        -1, // OW6_DGN3_5888_OW6_DGN3_L7_MAP, // 7
        -1, // OW6_DGN3_5888_OW6_DGN3_L8_MAP, // 8
        -1, // OW6_DGN3_5888_OW6_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 6 Dungeon 4. 
  { 
    "O6 D4",
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
        -1, //OW6_DGN4_6144_OW6_DGN4_L7_MAP, // 7
        -1, //OW6_DGN4_6144_OW6_DGN4_L8_MAP, // 8
        -1, //OW6_DGN4_6144_OW6_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 7 Dungeon 1. 
  { 
    "O7 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN1_6400_OW7_DGN1_L1_MAP, // 0
        OW7_DGN1_6400_OW7_DGN1_L1_MAP, // 1
        -1, //OW7_DGN1_6400_OW7_DGN1_L2_MAP, // 2
        -1, //OW7_DGN1_6400_OW7_DGN1_L3_MAP, // 3
        -1, //OW7_DGN1_6400_OW7_DGN1_L4_MAP, // 4
        -1, //OW7_DGN1_6400_OW7_DGN1_L5_MAP, // 5
        -1, // OW7_DGN1_6400_OW7_DGN1_L6_MAP, // 6
        -1, // OW7_DGN1_6400_OW7_DGN1_L7_MAP, // 7
        -1, // OW7_DGN1_6400_OW7_DGN1_L8_MAP, // 8
        -1, // OW7_DGN1_6400_OW7_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 7 Dungeon 2. 
  { 
    "O7 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN2_6656_OW7_DGN2_L1_MAP, // 0
        OW7_DGN2_6656_OW7_DGN2_L1_MAP, // 1
        -1, //OW7_DGN2_6656_OW7_DGN2_L2_MAP, // 2
        -1, //OW7_DGN2_6656_OW7_DGN2_L3_MAP, // 3
        -1, //OW7_DGN2_6656_OW7_DGN2_L4_MAP, // 4
        -1, //OW7_DGN2_6656_OW7_DGN2_L5_MAP, // 5
        -1, //OW7_DGN2_6656_OW7_DGN2_L6_MAP, // 6
        -1, //OW7_DGN2_6656_OW7_DGN2_L7_MAP, // 7
        -1, // OW7_DGN2_6656_OW7_DGN2_L8_MAP, // 8
        -1, //OW7_DGN2_6656_OW7_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 7 Dungeon 3. 
  { 
    "O7 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN3_6912_OW7_DGN3_L1_MAP, // 0
        OW7_DGN3_6912_OW7_DGN3_L1_MAP, // 1
        -1, //OW7_DGN3_6912_OW7_DGN3_L2_MAP, // 2
        -1, //OW7_DGN3_6912_OW7_DGN3_L3_MAP, // 3
        -1, //OW7_DGN3_6912_OW7_DGN3_L4_MAP, // 4
        -1, //OW7_DGN3_6912_OW7_DGN3_L5_MAP, // 5
        -1, //OW7_DGN3_6912_OW7_DGN3_L6_MAP, // 6
        -1, //OW7_DGN3_6912_OW7_DGN3_L7_MAP, // 7
        -1, //OW7_DGN3_6912_OW7_DGN3_L8_MAP, // 8
        -1, //OW7_DGN3_6912_OW7_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 7 Dungeon 4. 
  { 
    "O7 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW7_DGN4_7168_OW7_DGN4_L1_MAP, // 0
        OW7_DGN4_7168_OW7_DGN4_L1_MAP, // 1
        OW7_DGN4_7168_OW7_DGN4_L2_MAP, // 2
        OW7_DGN4_7168_OW7_DGN4_L3_MAP, // 3
        OW7_DGN4_7168_OW7_DGN4_L4_MAP, // 4
        OW7_DGN4_7168_OW7_DGN4_L5_MAP, // 5
        -1, //OW7_DGN4_7168_OW7_DGN4_L6_MAP, // 6
        -1, //OW7_DGN4_7168_OW7_DGN4_L7_MAP, // 7
        -1, //OW7_DGN4_7168_OW7_DGN4_L8_MAP, // 8
        -1, //OW7_DGN4_7168_OW7_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 8 Dungeon 1. 
  { 
    "O8 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN1_7424_OW8_DGN1_L1_MAP, // 0
        OW8_DGN1_7424_OW8_DGN1_L1_MAP, // 1
        OW8_DGN1_7424_OW8_DGN1_L2_MAP, // 2
        OW8_DGN1_7424_OW8_DGN1_L3_MAP, // 3
        -1, // OW8_DGN1_7424_OW8_DGN1_L4_MAP, // 4
        -1, // OW8_DGN1_7424_OW8_DGN1_L5_MAP, // 5
        -1, // OW8_DGN1_7424_OW8_DGN1_L6_MAP, // 6
        -1, // OW8_DGN1_7424_OW8_DGN1_L7_MAP, // 7
        -1, // OW8_DGN1_7424_OW8_DGN1_L8_MAP, // 8
        -1, // OW8_DGN1_7424_OW8_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 8 Dungeon 2. 
  { 
    "O8 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN2_7680_OW8_DGN2_L1_MAP, // 0
        OW8_DGN2_7680_OW8_DGN2_L1_MAP, // 1
        OW8_DGN2_7680_OW8_DGN2_L2_MAP, // 2
        -1, // OW8_DGN2_7680_OW8_DGN2_L3_MAP, // 3
        -1, // OW8_DGN2_7680_OW8_DGN2_L4_MAP, // 4
        -1, // OW8_DGN2_7680_OW8_DGN2_L5_MAP, // 5
        -1, // OW8_DGN2_7680_OW8_DGN2_L6_MAP, // 6
        -1, // OW8_DGN2_7680_OW8_DGN2_L7_MAP, // 7
        -1, // OW8_DGN2_7680_OW8_DGN2_L8_MAP, // 8
        -1, // OW8_DGN2_7680_OW8_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 8 Dungeon 3. 
  { 
    "O8 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN3_7936_OW8_DGN3_L1_MAP, // 0
        OW8_DGN3_7936_OW8_DGN3_L1_MAP, // 1
        OW8_DGN3_7936_OW8_DGN3_L2_MAP, // 2
        OW8_DGN3_7936_OW8_DGN3_L3_MAP, // 3
        OW8_DGN3_7936_OW8_DGN3_L4_MAP, // 4
        OW8_DGN3_7936_OW8_DGN3_L5_MAP, // 5
        -1, // OW8_DGN3_7936_OW8_DGN3_L6_MAP, // 6
        -1, // OW8_DGN3_7936_OW8_DGN3_L7_MAP, // 7
        -1, // OW8_DGN3_7936_OW8_DGN3_L8_MAP, // 8
        -1, // OW8_DGN3_7936_OW8_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 8 Dungeon 4. 
  { 
    "O8 D4",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW8_DGN4_8192_OW8_DGN4_L1_MAP, // 0
        OW8_DGN4_8192_OW8_DGN4_L1_MAP, // 1
        OW8_DGN4_8192_OW8_DGN4_L2_MAP, // 2
        OW8_DGN4_8192_OW8_DGN4_L3_MAP, // 3
        OW8_DGN4_8192_OW8_DGN4_L4_MAP, // 4
        -1, // OW8_DGN4_8192_OW8_DGN4_L5_MAP, // 5
        -1, // OW8_DGN4_8192_OW8_DGN4_L6_MAP, // 6
        -1, // OW8_DGN4_8192_OW8_DGN4_L7_MAP, // 7
        -1, // OW8_DGN4_8192_OW8_DGN4_L8_MAP, // 8
        -1, // OW8_DGN4_8192_OW8_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 9 Dungeon 1. 
  { 
    "O9 D1",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN1_8448_OW9_DGN1_L1_MAP, // 0
        OW9_DGN1_8448_OW9_DGN1_L1_MAP, // 1
        -1, // OW9_DGN1_8448_OW9_DGN1_L2_MAP, // 2
        -1, // OW9_DGN1_8448_OW9_DGN1_L3_MAP, // 3
        -1, // OW9_DGN1_8448_OW9_DGN1_L4_MAP, // 4
        -1, // OW9_DGN1_8448_OW9_DGN1_L5_MAP, // 5
        -1, // OW9_DGN1_8448_OW9_DGN1_L6_MAP, // 6
        -1, // OW9_DGN1_8448_OW9_DGN1_L7_MAP, // 7
        -1, // OW9_DGN1_8448_OW9_DGN1_L8_MAP, // 8
        -1, // OW9_DGN1_8448_OW9_DGN1_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 9 Dungeon 2. 
  { 
    "O9 D2",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN2_8704_OW9_DGN2_L1_MAP, // 0
        OW9_DGN2_8704_OW9_DGN2_L1_MAP, // 1
        -1, // OW9_DGN2_8704_OW9_DGN2_L2_MAP, // 2
        -1, // OW9_DGN2_8704_OW9_DGN2_L3_MAP, // 3
        -1, // OW9_DGN2_8704_OW9_DGN2_L4_MAP, // 4
        -1, // OW9_DGN2_8704_OW9_DGN2_L5_MAP, // 5
        -1, // OW9_DGN2_8704_OW9_DGN2_L6_MAP, // 6
        -1, // OW9_DGN2_8704_OW9_DGN2_L7_MAP, // 7
        -1, // OW9_DGN2_8704_OW9_DGN2_L8_MAP, // 8
        -1, // OW9_DGN2_8704_OW9_DGN2_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 9 Dungeon 3. 
  { 
    "O9 D3",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW9_DGN3_8960_OW9_DGN3_L1_MAP, // 0
        OW9_DGN3_8960_OW9_DGN3_L1_MAP, // 1
        OW9_DGN3_8960_OW9_DGN3_L2_MAP, // 2
        OW9_DGN3_8960_OW9_DGN3_L3_MAP, // 3
        -1, // OW9_DGN3_8960_OW9_DGN3_L4_MAP, // 4
        -1, // OW9_DGN3_8960_OW9_DGN3_L5_MAP, // 5
        -1, // OW9_DGN3_8960_OW9_DGN3_L6_MAP, // 6
        -1, // OW9_DGN3_8960_OW9_DGN3_L7_MAP, // 7
        -1, // OW9_DGN3_8960_OW9_DGN3_L8_MAP, // 8
        -1, // OW9_DGN3_8960_OW9_DGN3_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  
  // OVER WORLD 9 Dungeon 4. 
  { 
    "O9 D4",
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
        -1, // OW9_DGN4_9216_OW9_DGN4_L7_MAP, // 7
        -1, // OW9_DGN4_9216_OW9_DGN4_L8_MAP, // 8
        -1, // OW9_DGN4_9216_OW9_DGN4_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  },
  // OVER WORLD 9 Dungeon 5 (Also Dev Dungeon!!).
  {
    "O2 D6 (F)",
    {
      GLOBAL_OBJECT_LAYER_BMP,
      {
        OW2_DGN5_9472_OW2_DGN5_L1_MAP, // 0
        OW2_DGN5_9472_OW2_DGN5_L1_MAP, // 1
        OW2_DGN5_9472_OW2_DGN5_L2_MAP, // 2
        OW2_DGN5_9472_OW2_DGN5_L3_MAP, // 3
        OW2_DGN5_9472_OW2_DGN5_L4_MAP, // 4
        OW2_DGN5_9472_OW2_DGN5_L5_MAP, // 5
        OW2_DGN5_9472_OW2_DGN5_L6_MAP, // 6
        OW2_DGN5_9472_OW2_DGN5_L7_MAP, // 7
        OW2_DGN5_9472_OW2_DGN5_L8_MAP, // 8
        OW2_DGN5_9472_OW2_DGN5_L9_MAP, // 9
        -1 // EMPTY -- no level 10 evar!!
      },
    }
  }
};
#endif