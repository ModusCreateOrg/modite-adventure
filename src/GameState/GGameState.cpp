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

// scope local to this file.  If the value for the slot is ETrue, then the slot has already been remapped.
// We don't want to remap twice or the color range ends up in the wrong place.
static TBool slotRemapState[SLOT_MAX];

void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot, TUint aPalette, TUint16 aColors) {
  if (!slotRemapState[aSlot]) {
    gResourceManager.LoadBitmap(aBMP, aSlot, IMAGE_64x64);
  }
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  if (!slotRemapState[aSlot]) {
#ifdef DEBUGME
    printf("ReamapSlot(%d,%d,%d,%d)\n", aBMP, aSlot, aPalette, aColors);
#endif
    bm->Remap(aPalette, aColors);
    slotRemapState[aSlot] = ETrue;
  }
  gDisplay.SetPalette(bm->GetPalette(), aPalette, aColors);
}

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, 10 * 32) - 1, MIN(SCREEN_HEIGHT, 7 * 32) - 1));

  for (TInt i = 0; i < SLOT_MAX; i++) {
    slotRemapState[i] = EFalse;
  }

  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT, PLAYER_PALETTE, PLAYER_COLORS);
  mPlayerProcess = new GPlayerProcess(this);
  AddProcess(mPlayerProcess);
  LoadLevel(EXAMPLE_FILELIST_TXT_MAP);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
}

void GGameState::PreRender() {
  gDisplay.renderBitmap->Clear();
}

void GGameState::LoadLevel(TUint16 aTileMapId) {
  if (mPlayfield) {
    delete mPlayfield;
  }
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, EXAMPLE_FILELIST_TXT_MAP);

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
        RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT, PLAYER_PALETTE, PLAYER_COLORS);
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT, SPIDER_PALETTE, SPIDER_COLORS);
        AddProcess(new GSpiderProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_BAT:
        printf("BAT at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_BAT_BMP, BAT_SLOT, BAT_PALETTE, BAT_COLORS);
        AddProcess(new GBatProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN:
        printf("GOBLIN at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT, GOBLIN_PALETTE, GOBLIN_COLORS);
        AddProcess(new GGoblinProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_GOBLIN_SNIPER:
        printf("GOBLIN_SNIPER at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT, GOBLIN_SNIPER_PALETTE, GOBLIN_SNIPER_COLORS);
        AddProcess(new GGoblinSniperProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_ORC:
        printf("ORC at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_ORC_BMP, ORC_SLOT, ORC_PALETTE, ORC_COLORS);
        AddProcess(new GOrcProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_RAT:
        printf("RAT at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_RAT_BMP, RAT_SLOT, RAT_PALETTE, RAT_COLORS);
        AddProcess(new GRatProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_SLIME:
        printf("SLIME at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT, SLIME_PALETTE, SLIME_COLORS);
        AddProcess(new GSlimeProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      case ATTR_TROLL:
        printf("TROLL at %f,%f %d %d\n", xx, yy, op1, op2);
        RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT, TROLL_PALETTE, TROLL_COLORS);
        AddProcess(new GTrollProcess(this, mGamePlayfield, xx, yy + 63));
        break;
      default:
        printf("Invalid op code in Object Program: %x\n", program[ip]);
        break;
    }
  }
}

