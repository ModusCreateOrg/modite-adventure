// Game State
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GameState/player/GPlayerProcess.h"
#include "GameState/enemies/GSpiderProcess.h"
#include "GameState/enemies/GBatProcess.h"
#include "GameState/enemies/GGoblinProcess.h"
#include "GameState/enemies/GGoblinSniperProcess.h"
#include "GameState/enemies/GOrcProcess.h"
#include "GameState/enemies/GRatProcess.h"
#include "GameState/enemies/GSlimeProcess.h"
#include "GameState/enemies/GTrollProcess.h"

#define DEBUGME
#undef DEBUGME

static const TInt STATS_TIMER = 30 * 3;

// scope local to this file.  If the value for the slot is ETrue, then the slot
// has already been remapped. We don't want to remap twice or the color range
// ends up in the wrong place.
static TBool slotRemapState[SLOT_MAX];

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot) {
  if (!slotRemapState[aSlot]) {
    gResourceManager.LoadBitmap(aBMP, aSlot, IMAGE_64x64);
  }
  BBitmap *screen = mGamePlayfield->GetTilesBitmap();
  BBitmap *bm     = gResourceManager.GetBitmap(aSlot);
  if (!slotRemapState[aSlot]) {
#ifdef DEBUGME
    printf("ReamapSlot(%d,%d,%d,%d)\n", aBMP, aSlot, aPalette, aColors);
#endif
    bm->Remap(screen);
    slotRemapState[aSlot] = ETrue;
#ifdef DEBUGME
    printf(
        "Remapped bitmap, screen colors used %d\n", screen->CountUsedColors());
#endif
  }
  gDisplay.SetPalette(screen->GetPalette());

#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(UNDER_WATER_XM);
#endif
}

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1,
                           MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  for (TBool &i : slotRemapState) {
    i = EFalse;
  }

  mTimer         = STATS_TIMER;
  mStats         = EFalse;
  mPlayerProcess = ENull;
  mGamePlayfield = ENull;
  LoadLevel(DEVDUNGEON_0_LEVEL1_MAP);
  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
}

GGameState::~GGameState() { gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT); }

void GGameState::PreRender() { gDisplay.renderBitmap->Clear(COLOR_TEXT_BG); }

void GGameState::PostRender() {
  BViewPort vp;
  TRect     rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);

  const GAnchorSprite *player = PlayerSprite();
  char                output[160];
  if (--mTimer <= 0) {
    mStats = !mStats;
    mTimer = STATS_TIMER;
  }
  if (!mStats) {
    sprintf(output, "LVL: %2d HP: %3d GOLD: %4d", player->mLevel,
            player->mHitPoints, player->mGold);
  } else {
    sprintf(output, "EXP: %3d STR: %2d DEX: %2d", player->mExperience,
            player->mStrength, player->mDexterity);
  }
  gDisplay.renderBitmap->DrawString(
    &vp, output, gFont16x16, 0, 0, COLOR_TEXT, COLOR_TEXT_BG, -4);
}

TUint16 GGameState::MapWidth() {
  return (mGamePlayfield->MapWidthTiles() - TILES_WIDE) * 32;
}

TUint16 GGameState::MapHeight() {
  return (mGamePlayfield->MapHeightTiles() - TILES_HIGH) * 32;
}

GAnchorSprite *GGameState::PlayerSprite() { return mPlayerProcess->Sprite(); }

void GGameState::LoadLevel(TUint16 aTileMapId) {
  Reset(); // remove sprites and processes

  delete mPlayfield;

  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, aTileMapId);

  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
  RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT);
  RemapSlot(CHARA_BAT_BMP, BAT_SLOT);
  RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT);
  RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT);
  RemapSlot(CHARA_ORC_BMP, ORC_SLOT);
  RemapSlot(CHARA_RAT_BMP, RAT_SLOT);
  RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT);
  RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT);

  printf("Level loaded, colors used %d\n",
         mGamePlayfield->GetTilesBitmap()->CountUsedColors());

  mPlayerProcess = new GPlayerProcess(this);
  AddProcess(mPlayerProcess);

  TInt           objectCount = mGamePlayfield->mObjectCount;
  BObjectProgram *program    = mGamePlayfield->mObjectProgram;

  for (TInt ip = 0; ip < objectCount; ip++) {
    TUint16 op  = program[ip].mCode & TUint32(0xffff),
            op1 = program[ip].mRow,           // row
            op2 = program[ip].mCol;           // col

    auto xx = TFloat(op2 * 32), yy = TFloat(op1 * 32);

    switch (op) {
      case ATTR_PLAYER:
        printf("PLAYER at %.2f,%.2f\n", xx, yy);
        mPlayerProcess->StartLevel(mGamePlayfield, xx + 32, yy + 63);
        RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSpiderProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_BAT:
        printf("BAT at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GBatProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN:
        printf("GOBLIN at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GGoblinProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN_SNIPER:
        printf("GOBLIN_SNIPER at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(
          new GGoblinSniperProcess(this, mGamePlayfield, xx - 32, yy + 63));
        break;
      case ATTR_ORC:
        printf("ORC at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GOrcProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_RAT:
        printf("RAT at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GRatProcess(this, mGamePlayfield, xx - 18, yy + 63));
        break;
      case ATTR_SLIME:
        printf("SLIME at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSlimeProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_TROLL:
        printf("TROLL at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GTrollProcess(this, mGamePlayfield, xx - 20, yy + 63));
        break;
      case ATTR_STAIRS_UP:
        printf("STAIRS UP at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        break;
      case ATTR_STAIRS_DOWN:
        printf("STAIRS DOWN at %.2f,%.2f %d %d\n", xx, yy, op1, op2);
        break;
      default:
        printf("Invalid op code in Object Program: %x at %d,%d\n", program[ip].mCode, op1, op2);
        break;
    }
  }
}
