#include "GProcess.h"

#include "GameState/player/GPlayerProcess.h"
#include "GameState/enemies/GSpiderProcess.h"
#include "GameState/enemies/GBatProcess.h"
#include "GameState/enemies/GGoblinProcess.h"
#include "GameState/enemies/GGoblinSniperProcess.h"
#include "GameState/enemies/GOrcProcess.h"
#include "GameState/enemies/GRatProcess.h"
#include "GameState/enemies/GSlimeProcess.h"
#include "GameState/enemies/GTrollProcess.h"
#include "GameState/environment/GStairsProcess.h"
#include "GameState/environment/GOverworldLevelTransitionProcess.h"
#include "GameState/environment/GCrateProcess.h"
#include "GameState/environment/GChestProcess.h"
#include "GameState/environment/GSpikesProcess.h"
#include "GameState/environment/GDoorProcess.h"
#include "GameState/environment/GLeverProcess.h"
#include "GameState/environment/GFloorSwitchProcess.h"
#include "GameState/mid-bosses/GMidBossGenericProcess.h"

void GProcess::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mAttribute, sizeof(mAttribute));
  WriteCustomToStream(aStream); // write additional data, if any
  mSprite->WriteToStream(aStream);
}

void GProcess::ReadFromStream(BMemoryStream &aStream) {
  mSprite->ReadFromStream(aStream);
}

void GProcess::ReadCustomFromStream(BMemoryStream &aStream) {
  // we assume mAttribute was already read in
  ReadCustomFromStream(aStream);
  mSprite->ReadFromStream(aStream);
}

GProcess *GProcess::Spawn(GGameState *aGameState, TInt16 mAttribute, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams, DIRECTION aDirection, const char *aName) {
  TInt ip = aIp;
  TFloat xx = aX,
         yy = aY;
  TUint16 params = aParams;

  switch (mAttribute) {
    case ATTR_GONE:
    default:
      return ENull;
    case ATTR_STONE_STAIRS_UP:
      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_UP, aParams, xx, yy, "STONE"));

    case ATTR_OW_LEVEL_ENTRANCE:
      return aGameState->AddProcess(new GOverworldLevelTransitionProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "OVERWORLD"));
      break;

    case ATTR_STONE_STAIRS_DOWN:
      if (!strcasecmp(aName, "DUNGEON")) {
        return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "DUNGEON"));
      }
      else {
        return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "STONE"));
      }
      break;

    case ATTR_WOOD_STAIRS_UP:
      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_UP, aParams, xx, yy, "WOOD"));
      break;

    case ATTR_WOOD_STAIRS_DOWN:
      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, aParams, xx, yy, "WOOD"));
      break;

    case ATTR_CRATE:
      return aGameState->AddProcess(new GCrateProcess(aGameState, ip, params, xx, yy));
      break;

    case ATTR_CRATE_GONE:
      break;

    case ATTR_CHEST:
      return aGameState->AddProcess(new GChestProcess(aGameState, ip, params, xx, yy, EFalse));
    case ATTR_CHEST_OPEN:
      return aGameState->AddProcess(new GChestProcess(aGameState, ip, params, xx, yy, ETrue));
      break;
    case ATTR_SPIKES:
      return aGameState->AddProcess(new GSpikesProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_WOOD_DOOR_H:
      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, ETrue, ETrue));
      break;
    case ATTR_WOOD_DOOR_V:
      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, ETrue, EFalse));
      break;
    case ATTR_METAL_DOOR_H:
      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, EFalse, ETrue));
      break;
    case ATTR_METAL_DOOR_V:
      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, EFalse, EFalse));
      break;
    case ATTR_LEVER:
      return aGameState->AddProcess(new GLeverProcess(aGameState, ip, params, xx, yy));
      break;
    case ATTR_FLOOR_SWITCH:
      return aGameState->AddProcess(new GFloorSwitchProcess(aGameState, ip, params, xx, yy, EFalse));
      break;
    case ATTR_FLOOR_SWITCH_WOOD:
      return aGameState->AddProcess(new GFloorSwitchProcess(aGameState, ip, params, xx, yy, ETrue));
      break;
    case ATTR_PLAYER_IN1:
      // ignore
      break;
    case ATTR_SPIDER:
      return aGameState->AddProcess(new GSpiderProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_BAT:
      return aGameState->AddProcess(new GBatProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_GOBLIN:
      return aGameState->AddProcess(new GGoblinProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_GOBLIN_SNIPER:
      return aGameState->AddProcess(new GGoblinSniperProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_ORC:
      return aGameState->AddProcess(new GOrcProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_RAT:
      return aGameState->AddProcess(new GRatProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_SLIME:
      return aGameState->AddProcess(new GSlimeProcess(aGameState, ip, xx, yy, params));
      break;
    case ATTR_TROLL:
      return aGameState->AddProcess(new GTrollProcess(aGameState, ip, xx, yy, params));
      break;

      // mid bosses
    case ATTR_MID_BOSS_ENERGY:
      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, MID_BOSS_SLOT, ip, params, MID_BOSS_ENERGY_BMP_SPRITES));
    case ATTR_MID_BOSS_FIRE:
      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, MID_BOSS_SLOT, ip, params, MID_BOSS_FIRE_BMP_SPRITES));
    case ATTR_MID_BOSS_EARTH:
      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, MID_BOSS_SLOT, ip, params, MID_BOSS_EARTH_BROWN_BMP_SPRITES));
    case ATTR_MID_BOSS_WATER:
      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, MID_BOSS_SLOT, ip, params, MID_BOSS_WATER_BMP_SPRITES));
      break;
  }
  return ENull;
}
