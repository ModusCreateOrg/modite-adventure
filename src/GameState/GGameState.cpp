// Game State
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GResources.h"
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

const TInt GAUGE_WIDTH = 90;

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

// scope local to this file.  If the value for the slot is ETrue, then the slot
// has already been remapped. We don't want to remap twice or the color range
// ends up in the wrong place.

static TBool slotRemapState[SLOT_MAX];

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot, TInt16 aImageSize) {

  if (!slotRemapState[aSlot]) {
    gResourceManager.ReleaseBitmapSlot(aSlot);
    gResourceManager.LoadBitmap(aBMP, aSlot, aImageSize);
  }
  BBitmap *screen = mGamePlayfield->GetTilesBitmap();
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  if (!slotRemapState[aSlot]) {
    bm->Remap(screen);
    slotRemapState[aSlot] = ETrue;
#ifdef DEBUGME
    printf("Remapped bitmap %d, screen colors used %d\n", aBMP, screen->CountUsedColors());
#endif
  }
  gDisplay.SetPalette(screen->GetPalette());

#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(UNDER_WATER_XM);
#endif
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

// Constructor
GGameState::GGameState() : BGameEngine(gViewPort), mText(""), mName(""), mLevel(0), mNextLevel(0), mTileMapId(0),
                           mNextTileMapId(0) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1,
                           MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  mTimer = FRAMES_PER_SECOND * 1;

  mGamePlayfield = ENull;
  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  GPlayer::Init();
  LoadLevel("Dungeon0", 1, DEVDUNGEON_0_LEVEL2_MAP);
}

GGameState::~GGameState() { gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT); }

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::PreRender() {
  gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);
  if (mNextLevel != mLevel) {
    LoadLevel(mName, mNextLevel, mNextTileMapId);
  }
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

static void fuel_gauge(BViewPort *vp, TInt x, TInt y, TInt stat, TInt stat_max, TUint8 color) {
  BBitmap *screen = gDisplay.renderBitmap;

  // calculate fill percentage
  TRect r(0, 0, GAUGE_WIDTH, 8);

  // offset to display coordinates
  r.Offset(x, y);

  // draw frame
  screen->DrawRect(vp, r, COLOR_TEXT);

  // calculate percentage
  TFloat pct = stat_max ? (TFloat(stat)) / TFloat(stat_max) : 0.;

  // fill area
  TRect fill(0, 0, TInt(pct * GAUGE_WIDTH), 8);
  fill.x1 += 2;
  fill.y1 += 2;
  fill.x2 -= 2;
  fill.y2 -= 2;
  if (fill.x2 < fill.x1) {
    fill.x2 = fill.x1 + 1;
  }
  fill.Offset(x, y);
  screen->FillRect(vp, fill, color);
}

void GGameState::PostRender() {
  if (mText[0]) {
    TInt len = strlen(mText);
    TInt x = gViewPort->mRect.Width() / 2 - len * 12 / 2;
    gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont16x16, x, 32, COLOR_SHMOO, COLOR_TEXT_TRANSPARENT, -4);
    if (--mTimer < 0) {
      mText[0] = '\0';
      gControls.Reset();
      Enable();
    }
    return;
  }

  BViewPort vp;
  TRect rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);

  BBitmap *b = gResourceManager.GetBitmap(PLAYER_SLOT),
    *screen = gDisplay.renderBitmap;

  const TInt BOTTLE_X = 64 * 3,
    BOTTLE_Y = 14,
    BOTTLE_WIDTH = 12,
    BOTTLE_HEIGHT = 15;

  TInt x = 2;

  // render health potion
  TRect healing(BOTTLE_X, BOTTLE_Y, BOTTLE_X + BOTTLE_WIDTH, BOTTLE_Y + BOTTLE_HEIGHT);
  switch (GPlayer::mHealthPotion) {
    case 75:
      healing.Offset(BOTTLE_WIDTH * 1, 0);
      break;
    case 50:
      healing.Offset(BOTTLE_WIDTH * 2, 0);
      break;
    case 25:
      healing.Offset(BOTTLE_WIDTH * 3, 0);
      break;
    case 0:
      healing.Offset(BOTTLE_WIDTH * 4, 0);
    default:
      break;
  }
  screen->DrawBitmapTransparent(&vp, b, healing, x, 1);
  x += 16;

  // render mana potion
  TRect mana(BOTTLE_X, BOTTLE_Y + BOTTLE_HEIGHT + 2, BOTTLE_X + BOTTLE_WIDTH, BOTTLE_Y + BOTTLE_HEIGHT + BOTTLE_HEIGHT + 2);
  switch (GPlayer::mManaPotion) {
    case 75:
      mana.Offset(BOTTLE_WIDTH * 1, 0);
      break;
    case 50:
      mana.Offset(BOTTLE_WIDTH * 2, 0);
      break;
    case 25:
      mana.Offset(BOTTLE_WIDTH * 3, 0);
      break;
    case 0:
      mana.Offset(BOTTLE_WIDTH * 4, 0);
    default:
      break;
  }
  screen->DrawBitmapTransparent(&vp, b, mana, x, 1);
  x += 16;

  // render heart
  TRect heart(64 * 3, 0, 64 * 3 + 15, 11);
  screen->DrawBitmapTransparent(&vp, b, heart, x, 3);
  x += 18;

  // health fuel gauge
  gDisplay.SetColor(COLOR_HEALTH, 255, 0, 0);
  fuel_gauge(&vp, x, 4, GPlayer::mHitPoints, GPlayer::mMaxHitPoints, COLOR_HEALTH);
  x += GAUGE_WIDTH + 8;

  // experience fuel gauge
  gDisplay.SetColor(COLOR_EXPERIENCE, 0, 255, 0);
  screen->DrawString(&vp, "XP", gFont16x16, x, 0, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
  x += 28;
  fuel_gauge(&vp, x, 4, GPlayer::mExperience, GPlayer::mNextLevel, COLOR_EXPERIENCE);
  x += GAUGE_WIDTH + 8;

  // display level
  char output[160];
  const TInt l_width = 48 + 2; // 2 px padding right
  sprintf(output, "L%-3d", GPlayer::mLevel);
  screen->DrawString(&vp, output, gFont16x16, x, 0, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

TUint16 GGameState::MapWidth() {
  return (mGamePlayfield->MapWidthTiles() - gViewPort->mRect.Width() / 32) * 32;
}

TUint16 GGameState::MapHeight() {
  return (mGamePlayfield->MapHeightTiles() - gViewPort->mRect.Height() / 32) * 32;
}

GAnchorSprite *GGameState::PlayerSprite() { return GPlayer::mSprite; }

void GGameState::GameLoop() {
  for (TInt s=0; s<16; s++) {
    mGamePlayfield->mGroupState[s] = ETrue;
  }

  BGameEngine::GameLoop();

  for (TInt s=0; s<16; s++) {
    if (mGamePlayfield->mGroupState[s] == ETrue) {
      mGamePlayfield->mGroupDone[s] = ETrue;
    }
  }
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

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
  RemapSlot(DUNGEON_TILESET_OBJECTS_BMP, ENVIRONMENT_SLOT, IMAGE_32x32);
  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
  RemapSlot(CHARA_HERO_HEAL_EFFECT_BMP, PLAYER_HEAL_SLOT, IMAGE_32x32);
  RemapSlot(CHARA_HERO_SPELL_EFFECT_BMP, PLAYER_SPELL_SLOT, IMAGE_32x32);
  RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT);
  RemapSlot(CHARA_BAT_BMP, BAT_SLOT);
  RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT);
  RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT);
  RemapSlot(CHARA_ORC_BMP, ORC_SLOT);
  RemapSlot(CHARA_RAT_BMP, RAT_SLOT);
  RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT);
  RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT);
  RemapSlot(ENEMY_DEATH_BMP, ENEMY_DEATH_SLOT, IMAGE_32x32);

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
  TInt eCount = 0;
  for (TInt ip = 0; ip < objectCount; ip++) {
    TUint16 op = program[ip].mCode & TUint32(0xffff),
      params = program[ip].mCode >> TUint32(16),
      row = program[ip].mRow, // row
      col = program[ip].mCol;     // col

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
        printf("FLOOR_SWITCH at %.2f,%.2f %d %d params: %x\n", xx, yy, row, col, params);
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
        AddProcess(new GGoblinSniperProcess(this, xx - 32, yy + 32, params));
        break;
      case ATTR_ORC:
        // TODO @jaygarcia Using our test level 1, we spawn 2+ ORCs
        // to test other enemy logic, comment out spawning the Orc and instead spawn the enemy we want to see/test
        printf("ORC at %.2f,%.2f %d %d\n", xx, yy, row, col);
        //        AddProcess(new GGoblinProcess(this, xx, yy + 32, params));
        AddProcess(new GOrcProcess(this, xx, yy + 32, params));
        break;
      case ATTR_RAT:
        printf("RAT at %.2f,%.2f %d %d %d\n", xx, yy, row, col, eCount);
        AddProcess(new GRatProcess(this, xx - 18, yy + 31, params));
        break;
      case ATTR_SLIME:
        printf("SLIME at %.2f,%.2f %d %d\n", xx, yy, row, col);
        AddProcess(new GSlimeProcess(this, xx - 18, yy + 31, params));
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
