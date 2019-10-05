// Game State
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GameState/status/GStartLevelProcess.h"
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
#include "GameState/environment/GCrateProcess.h"
#include "GameState/environment/GChestProcess.h"
#include "GameState/environment/GSpikesProcess.h"
#include "GameState/environment/GDoorProcess.h"
#include "GameState/environment/GLeverProcess.h"
#include "GameState/environment/GFloorSwitchProcess.h"

#include "GPlayer.h"


#define DEBUGME
//#undef DEBUGME

static const TInt STATS_TIMER = FRAMES_PER_SECOND * 3;

// scope local to this file.  If the value for the slot is ETrue, then the slot
// has already been remapped. We don't want to remap twice or the color range
// ends up in the wrong place.
static TBool slotRemapState[SLOT_MAX];

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot) {
  if (!slotRemapState[aSlot]) {
    gResourceManager.ReleaseBitmapSlot(aSlot);
    gResourceManager.LoadBitmap(aBMP, aSlot, aSlot == ENVIRONMENT_SLOT ? IMAGE_32x32 : IMAGE_64x64);
  }
  BBitmap *screen = mGamePlayfield->GetTilesBitmap();
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  if (!slotRemapState[aSlot]) {
    bm->Remap(screen);
    slotRemapState[aSlot] = ETrue;
#ifdef DEBUGME
    printf( "Remapped bitmap %d, screen colors used %d\n", aBMP, screen->CountUsedColors());
#endif
  }
  gDisplay.SetPalette(screen->GetPalette());

#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(UNDER_WATER_XM);
#endif
}

GGameState::GGameState() : BGameEngine(gViewPort), mText(""), mName(""), mLevel(0), mNextLevel(0), mTileMapId(0), mNextTileMapId(0) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1,
                           MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  mTimer = FRAMES_PER_SECOND * 1;
  mStats = EFalse;

  mGamePlayfield = ENull;
  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);

  gDisplay.UseColor(COLOR_TEXT_BG);
  gDisplay.UseColor(COLOR_TEXT);

  LoadLevel("Dungeon0", 1, DEVDUNGEON_0_LEVEL1_MAP);
}

GGameState::~GGameState() { gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT); }

void GGameState::PreRender() {
  gDisplay.renderBitmap->Clear(BLACK);
  if (mNextLevel != mLevel) {
    LoadLevel(mName, mNextLevel, mNextTileMapId);
  }
}

void GGameState::PostRender() {
  gDisplay.SetColor(COLOR_TEXT_BG, BLACK);
  gDisplay.SetColor(COLOR_TEXT, WHITE);

  if (mText[0]) {
    TInt len = strlen(mText);
    TInt x = gViewPort->mRect.Width() / 2 - len * 12 / 2;
    gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont16x16, x, 32, WHITE, -4);
    if (--mTimer < 0) {
      mText[0] = '\0';
      mTimer = STATS_TIMER;
      gControls.Reset();
      Enable();
    }
    return;
  }

  BViewPort vp;
  TRect rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);

  if (--mTimer <= 0) {
    mStats = !mStats;
    mTimer = STATS_TIMER;
  }
  char output[160];
  if (!mStats) {
    sprintf(output, "LVL: %2d HP: %3d GOLD: %4d", GPlayer::mLevel, GPlayer::mHitPoints, GPlayer::mGold);
  } else {
    sprintf(output, "EXP: %3d STR: %2d DEX: %2d", GPlayer::mExperience, GPlayer::mStrength, GPlayer::mDexterity);
  }
  gDisplay.renderBitmap->DrawString(&vp, output, gFont16x16, 0, 0, WHITE, BLACK, -4);
}

TUint16 GGameState::MapWidth() {
  return (mGamePlayfield->MapWidthTiles() - gViewPort->mRect.Width() / 32) * 32;
}

TUint16 GGameState::MapHeight() {
  return (mGamePlayfield->MapHeightTiles() - gViewPort->mRect.Height() / 32) * 32;
}

GAnchorSprite *GGameState::PlayerSprite() { return GPlayer::mSprite; }


void GGameState::NextLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId) {
  mNextLevel = aLevel;
  strcpy(mName, aName);
  mNextTileMapId = aTileMapId;
  mTimer = 1 * FRAMES_PER_SECOND;
  sprintf(mText, "%s Level %d", aName, aLevel);
}

void GGameState::LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId) {
  strcpy(mName, aName);
  mLevel = mNextLevel = aLevel;
  mTileMapId = aTileMapId;

  Reset(); // remove sprites and processes
  GPlayer::mProcess = ENull;
  for (TBool &i : slotRemapState) {
    i = EFalse;
  }

  delete mPlayfield;

  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, aTileMapId);
  sprintf(mText, "%s Level %d", aName, aLevel);
  mTimer = 1 * FRAMES_PER_SECOND;
  Disable();

//  AddProcess(new GStartLevelProcess(aName, aLevel));
  RemapSlot(DUNGEON_TILESET_OBJECTS_BMP, ENVIRONMENT_SLOT);
  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
  RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT);
  RemapSlot(CHARA_BAT_BMP, BAT_SLOT);
  RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT);
  RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT);
  RemapSlot(CHARA_ORC_BMP, ORC_SLOT);
  RemapSlot(CHARA_RAT_BMP, RAT_SLOT);
  RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT);
  RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT);

  GPlayer::mProcess = new GPlayerProcess(this);
  AddProcess(GPlayer::mProcess);

  printf("Level loaded, colors used %d\n",
         mGamePlayfield->GetTilesBitmap()->CountUsedColors());

  TInt objectCount = mGamePlayfield->mObjectCount;
  BObjectProgram *program = mGamePlayfield->mObjectProgram;

  // count spikes
  GSpikesProcess::mNumber = 0;
  for (TInt ip = 0; ip < objectCount; ip++) {
    TUint16 op = program[ip].mCode & TUint32(0xffff);

    if (op == ATTR_SPIKES) {
      GSpikesProcess::mNumber++;
    }
  }

  TBool startedPlayer = EFalse;
  TInt16 spikes_number = GSpikesProcess::mNumber;
  for (TInt ip = 0; ip < objectCount; ip++) {
    TUint16 op = program[ip].mCode & TUint32(0xffff),
      params = program[ip].mCode >> TUint32(16),
      row = program[ip].mRow,           // row
      col = program[ip].mCol;           // col

    auto xx = TFloat(col * 32), yy = TFloat(row * 32);

    switch (op) {
      case ATTR_STONE_STAIRS_UP:
        printf("STONE STAIRS UP at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GStairsProcess(this, DIRECTION_UP, params, xx, yy, "STONE"));
        break;
      case ATTR_STONE_STAIRS_DOWN:
        printf("STONE STAIRS DOWN at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GStairsProcess(this, DIRECTION_DOWN, params, xx, yy, "STONE"));
        break;
      case ATTR_WOOD_STAIRS_UP:
        printf("WOOD STAIRS UP at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GStairsProcess(this, DIRECTION_UP, params, xx, yy, "WOOD"));
        break;
      case ATTR_WOOD_STAIRS_DOWN:
        printf("WOOD STAIRS DOWN at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GStairsProcess(this, DIRECTION_DOWN, params, xx, yy, "WOOD"));
        break;
      case ATTR_CRATE:
        printf("CRATE at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GCrateProcess(this, params, xx, yy));
        break;
      case ATTR_CHEST:
        printf("CHEST at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GChestProcess(this, params, xx, yy));
        break;
      case ATTR_SPIKES:
        printf("SPIKES at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GSpikesProcess(this, xx, yy + 30, spikes_number--));
        break;
      case ATTR_METAL_DOOR_H:
        printf("METAL DOOR H at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GDoorProcess(this, params, xx, yy + 30, EFalse, ETrue));
        break;
      case ATTR_METAL_DOOR_V:
        printf("METAL DOOR V at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GDoorProcess(this, params, xx, yy + 30, EFalse, EFalse));
        break;
      case ATTR_WOOD_DOOR_H:
        printf("WOOD DOOR H at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GDoorProcess(this, params, xx, yy + 30, ETrue, ETrue));
        break;
      case ATTR_WOOD_DOOR_V:
        printf("WOOD DOOR V at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GDoorProcess(this, params, xx, yy + 30, ETrue, EFalse));
        break;
      case ATTR_LEVER:
        printf("LEVER at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GLeverProcess(this, params, xx, yy + 32));
        break;
      case ATTR_FLOOR_SWITCH:
        printf("FLOOR_SWITCH at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GFloorSwitchProcess(this, params, xx, yy + 32, EFalse));
        break;
      case ATTR_FLOOR_SWITCH_WOOD:
        printf("FLOOR_SWITCH at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GFloorSwitchProcess(this, params, xx, yy + 32, ETrue));
        break;

      case ATTR_PLAYER:
        printf("PLAYER at %.2f,%.2f\n", xx, yy);
        GPlayer::mProcess->StartLevel(mGamePlayfield, xx - 16, yy + 32);
        startedPlayer = ETrue;
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %.2f,%.2f %d %d\n", xx - 32, yy, row, col);
        AddProcess(new GSpiderProcess(this, xx - 32, yy + 32, params));
        break;
      case ATTR_BAT:
        printf("BAT at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GBatProcess(this, xx - 32, yy + 32, params));
        break;
      case ATTR_GOBLIN:
        printf("GOBLIN at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GGoblinProcess(this, xx, yy + 32, params));
        break;
      case ATTR_GOBLIN_SNIPER:
        printf("GOBLIN_SNIPER at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(
          new GGoblinSniperProcess(this, xx - 32, yy + 32, params));
        break;
      case ATTR_ORC:
        printf("ORC at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GOrcProcess(this, xx, yy + 32, params));
        break;
      case ATTR_RAT:
        printf("RAT at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GRatProcess(this, xx - 18, yy + 31, params));
        break;
      case ATTR_SLIME:
        printf("SLIME at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GSlimeProcess(this, xx, yy + 32, params));
        break;
      case ATTR_TROLL:
        printf("TROLL at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GTrollProcess(this, xx - 20, yy + 32, params));
        break;
      default:
        printf("Invalid op code in Object Program: %x at col,row %d,%d\n", program[ip].mCode, col, row);
        break;
    }
  }
  if (!startedPlayer) {
    printf("NO PLAYER at %.2f,%.2f\n", 32., 64.);
    GPlayer::mProcess->StartLevel(mGamePlayfield, 32. + 32, 64. + 63);
  }
}
