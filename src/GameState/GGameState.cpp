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

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, 10 * 32) - 1, MIN(SCREEN_HEIGHT, 7 * 32) - 1));

  // load and remap player
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  BBitmap *bm = gResourceManager.GetBitmap(PLAYER_SLOT);
  bm->Remap(PLAYER_PALETTE, PLAYER_COLORS);

  // load and remap spider
  gResourceManager.LoadBitmap(CHARA_SPIDER_BMP, SPIDER_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(SPIDER_SLOT);
  bm->Remap(SPIDER_PALETTE, SPIDER_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_BAT_BMP, BAT_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(BAT_SLOT);
  bm->Remap(BAT_PALETTE, BAT_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_GOBLIN_BMP, GOBLIN_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(GOBLIN_SLOT);
  bm->Remap(GOBLIN_PALETTE, GOBLIN_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(GOBLIN_SNIPER_SLOT);
  bm->Remap(GOBLIN_SNIPER_PALETTE, GOBLIN_SNIPER_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_ORC_BMP, ORC_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(ORC_SLOT);
  bm->Remap(ORC_PALETTE, ORC_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_RAT_BMP, RAT_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(RAT_SLOT);
  bm->Remap(RAT_PALETTE, RAT_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_SLIME_BMP, SLIME_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(SLIME_SLOT);
  bm->Remap(SLIME_PALETTE, SLIME_COLORS);

  // load and remap bat
  gResourceManager.LoadBitmap(CHARA_TROLL_BMP, TROLL_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(TROLL_SLOT);
  bm->Remap(TROLL_PALETTE, TROLL_COLORS);

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

  TInt objectCount = mGamePlayfield->mObjectCount;
  TUint16 *program = mGamePlayfield->mObjectProgram;

  for (TInt ip = 0; ip < objectCount*3; ip += 3) {
    TUint16 op = program[ip],
      op1 = program[ip + 1],  // row
      op2 = program[ip + 2];  // col

    auto xx = TFloat(op2 * 32),
      yy = TFloat(op1 * 32);

    switch (op) {
      case ATTR_PLAYER:
        printf("PLAYER at %f,%f\n", xx, yy);
        mPlayerProcess->StartLevel(mGamePlayfield, xx-16, yy);
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSpiderProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_BAT:
        printf("BAT at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GBatProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_GOBLIN:
        printf("GOBLIN at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GGoblinProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_GOBLIN_SNIPER:
        printf("GOBLIN_SNIPER at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GGoblinSniperProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_ORC:
        printf("ORC at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GOrcProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_RAT:
        printf("RAT at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GRatProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_SLIME:
        printf("SLIME at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSlimeProcess(this, mGamePlayfield, xx, yy+63));
        break;
      case ATTR_TROLL:
        printf("TROLL at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GTrollProcess(this, mGamePlayfield, xx, yy+63));
        break;
      default:
        printf("Invalid op code in Object Program: %x\n", program[ip]);
        break;
    }
  }
}

