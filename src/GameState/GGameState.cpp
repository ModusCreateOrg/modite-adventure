#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"
#include "GSpiderProcess.h"
#include "GBatProcess.h"
#include "GGoblinProcess.h"
#include "GGoblinSniperProcess.h"
#include "GOrcProcess.h"
#include "GRatProcess.h"
#include "GSlimeProcess.h"
#include "GTrollProcess.h"

#define DEBUGME
#undef DEBUGME

// scope local to this file.  If the value for the slot is ETrue, then the slot has already been remapped.
// We don't want to remap twice or the color range ends up in the wrong place.
static TBool slotRemapState[SLOT_MAX];

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot) {
  if (!slotRemapState[aSlot]) {
    gResourceManager.LoadBitmap(aBMP, aSlot, IMAGE_64x64);
  }
  BBitmap *screen = mGamePlayfield->GetTilesBitmap();
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  if (!slotRemapState[aSlot]) {
#ifdef DEBUGME
    printf("ReamapSlot(%d,%d,%d,%d)\n", aBMP, aSlot, aPalette, aColors);
#endif
    bm->Remap(screen);
    slotRemapState[aSlot] = ETrue;
#ifdef DEBUGME
    printf("Remapped bitmap, screen colors used %d\n", screen->CountUsedColors());
#endif
  }
  gDisplay.SetPalette(screen->GetPalette());
}

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1, MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  for (TBool & i : slotRemapState) {
    i = EFalse;
  }

  mPlayerProcess = ENull;
  mGamePlayfield = ENull;
  LoadLevel(EXAMPLE_FILELIST_TXT_MAP);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
}

void GGameState::PreRender() {
  gDisplay.renderBitmap->Clear();
}

TUint16 GGameState::MapWidth() {
  return (mGamePlayfield->MapWidthTiles() - TILES_WIDE) * 32;
}

TUint16 GGameState::MapHeight() {
  return (mGamePlayfield->MapHeightTiles() - TILES_HIGH) * 32;
}

void GGameState::LoadLevel(TUint16 aTileMapId) {
  Reset();  // remove sprites and processes

  if (mPlayfield) {
    delete mPlayfield;
  }
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, EXAMPLE_FILELIST_TXT_MAP);

  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
  RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT);
  RemapSlot(CHARA_BAT_BMP, BAT_SLOT);
  RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT);
  RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT);
  RemapSlot(CHARA_ORC_BMP, ORC_SLOT);
  RemapSlot(CHARA_RAT_BMP, RAT_SLOT);
  RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT);
  RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT);

  printf("Level loaded, colors used %d\n", mGamePlayfield->GetTilesBitmap()->CountUsedColors());

  mPlayerProcess = new GPlayerProcess(this);
  AddProcess(mPlayerProcess);

  TInt    objectCount = mGamePlayfield->mObjectCount;
  TUint16 *program    = mGamePlayfield->mObjectProgram;

  for (TInt ip = 0; ip < objectCount * 3; ip += 3) {
    TUint16 op  = program[ip],
            op1 = program[ip + 1],  // row
            op2 = program[ip + 2];  // col

    auto xx = TFloat(op2 * 32),
         yy = TFloat(op1 * 32);

    switch (op) {
      case ATTR_PLAYER:
        printf("PLAYER at %f,%f\n", xx, yy);
        mPlayerProcess->StartLevel(mGamePlayfield, xx - 16, yy);
        RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSpiderProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_BAT:
        printf("BAT at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GBatProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN:
        printf("GOBLIN at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GGoblinProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN_SNIPER:
        printf("GOBLIN_SNIPER at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GGoblinSniperProcess(this, mGamePlayfield, xx-32, yy + 63));
        break;
      case ATTR_ORC:
        printf("ORC at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GOrcProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_RAT:
        printf("RAT at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GRatProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_SLIME:
        printf("SLIME at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSlimeProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_TROLL:
        printf("TROLL at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GTrollProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      default:
        printf("Invalid op code in Object Program: %x\n", program[ip]);
        break;
    }
  }
}

