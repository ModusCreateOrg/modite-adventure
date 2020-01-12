// Game State
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GResources.h"
#include "GameState/player/GGameOver.h"
#include "GameState/environment/GSpikesProcess.h"
#include "GameState/player/GPlayerProcess.h"

#include "GPlayer.h"

#include <BMemoryStream.h>

#define DEBUGME
//#undef DEBUGME

const TInt GAUGE_WIDTH = 90;

// info about the dungeons
#include "DungeonDefs.h"
const TInt NUM_DUNGEONS = sizeof(gDungeonDefs) / sizeof(TDungeonInfo);

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::Init() {
  strcpy(mText, "");
  strcpy(mName, "");
  mLevel = 0;
  mNextDungeon = mDungeon = 0;
  mNextLevel = 0;
  mTileMapId = 0;
  mPlayerToLoad = ATTR_PLAYER_IN1;

  mNextTileMapId = 0;
  mNextObjectsId = 0;

  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1, MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  mTimer = FRAMES_PER_SECOND * 1;
  mGameOver = ENull;

  mGamePlayfield = mNextGamePlayfield = ENull;

  // Clear BObject programs
  GGamePlayfield::ResetCache();

  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  GPlayer::Init();
}

// Constructor
GGameState::GGameState() : BGameEngine(gViewPort) {
  Init();
  for (TInt i = 2; i < SLOT_MAX; i++) {
    if (gResourceManager.GetBitmap(i)) {
      printf("Releasing slot %d\n", i);
      gResourceManager.ReleaseBitmapSlot(i);
    }
    mSlotRemapState[i] = EFalse;
  }
  NextLevel(OVERWORLD_DUNGEON, 1);
}

// Constructor
GGameState::GGameState(const char *aName) : BGameEngine(gViewPort) {
  Init();
  for (TInt i = 2; i < SLOT_MAX; i++) {
    if (gResourceManager.GetBitmap(i)) {
      printf("Releasing slot %d\n", i);
      gResourceManager.ReleaseBitmapSlot(i);
    }
    mSlotRemapState[i] = EFalse;
  }
  LoadState(aName);
}

GGameState::~GGameState() {}

GProcess *GGameState::AddProcess(GProcess *p) {
  mProcessList.AddProcess(p);
  return p;
}

void GGameState::TryAgain(TBool aExitDungeon) {
  if (mGameOver) {
    delete mGameOver;
    mGameOver = ENull;
  }
  GPlayer::mGameOver = EFalse;
  GPlayer::mHitPoints = GPlayer::mMaxHitPoints;

  if (aExitDungeon) {
    NextLevel(OVERWORLD_DUNGEON, LastOverworldLevel());
    return;
  }

  // Cache previous respawn coords, LoadLevel resets them
  TFloat respawnAt[2] = { GPlayerProcess::mRespawnAt[0], GPlayerProcess::mRespawnAt[1] };

  // Reset the level
  LoadLevel(mName, mLevel, mTileMapId);

  // Restore respawn coords
  GPlayerProcess::mRespawnAt[0] = respawnAt[0];
  GPlayerProcess::mRespawnAt[1] = respawnAt[1];

  if (respawnAt[0] != '\0') {
    GAnchorSprite *s = GPlayer::mProcess->Sprite();
    s->x = respawnAt[0];
    s->y = respawnAt[1];
#ifdef DEBUG_MODE
    printf("RESPAWNED AT %f %f\n", respawnAt[0], respawnAt[1]);
#endif
  }
}

TBool GGameState::IsBossRoom() {
  TInt objectCount = mGamePlayfield->mObjectCount;
  BObjectProgram *program = mGamePlayfield->mObjectProgram;
  for (TInt ip = 0; ip < objectCount; ip++) {
    TUint16 op = program[ip].mCode & TUint32(0xffff);
    switch (op) {
      case ATTR_MID_BOSS_EARTH:
      case ATTR_MID_BOSS_ENERGY:
      case ATTR_MID_BOSS_FIRE:
      case ATTR_MID_BOSS_WATER:
      case ATTR_WIZARD_EARTH:
      case ATTR_WIZARD_ENERGY:
      case ATTR_WIZARD_FIRE:
      case ATTR_WIZARD_WATER:
      case ATTR_FINAL_BOSS:
        return ETrue;
      default:
        continue;
    }
  }

  return EFalse;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::PreRender() {
  if (mNextLevel != mLevel || mNextDungeon != mDungeon) {
    if (mLevel) {
      mGamePlayfield->StartMosaicOut();

      if (mGamePlayfield->MosaicDone()) {
        LoadLevel(mName, mNextLevel, mNextTileMapId);
      }
    }
    else {
      LoadLevel(mName, mNextLevel, mNextTileMapId);
    }
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
  TFloat gw = pct * GAUGE_WIDTH;

  // fill area
  TRect fill(0, 0, TInt(gw), 8);
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
#ifdef DEBUG_MODE
  if (mText[0]) {
    TInt len = strlen(mText);
    gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont8x8, 4, gViewPort->mRect.Height() - 10, COLOR_TEXT,
      COLOR_TEXT_TRANSPARENT);
  }
#endif

  BViewPort vp;
  TRect rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.renderBitmap->FillRect(&vp, vp.mRect, COLOR_TEXT_BG);

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
  TRect mana(BOTTLE_X, BOTTLE_Y + BOTTLE_HEIGHT + 2, BOTTLE_X + BOTTLE_WIDTH,
    BOTTLE_Y + BOTTLE_HEIGHT + BOTTLE_HEIGHT + 2);
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
  fuel_gauge(&vp, x, 4, GPlayer::mHitPoints, GPlayer::mMaxHitPoints,
    GPlayer::mSprite->mInvulnerable ? COLOR_SHMOO_RED : COLOR_HEALTH);
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

  if (mGameOver) {
    mGameOver->Run();
    return;
  }

  if (mGamePlayfield->MosaicActive()) {
    gControls.Reset();
    Pause();
  }
  else if (mGamePlayfield->MosaicDone()) {
    Resume();
  }
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

void GGameState::GameLoop() {
  for (bool &s : mGamePlayfield->mGroupState) {
    s = ETrue;
  }

  BGameEngine::GameLoop();

  for (TInt s = 0; s < 16; s++) {
    if (mGamePlayfield->mGroupState[s] == ETrue) {
      mGamePlayfield->mGroupDone[s] = ETrue;
    }
  }
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

/**
 * This is safe to call from BProcess context.
 *
 * @param aDungeon  ID of dungeon (in gDungeonDefs)
 * @param aLevel    Level in dungeon
 */
void GGameState::NextLevel(const TInt16 aDungeon, const TInt16 aLevel) {
  if (aDungeon == -1) {
    // -1 means stay in the same dungeon
    mNextDungeon = mDungeon;

    if (mNextDungeon != OVERWORLD_DUNGEON) {
      if (aLevel > mLevel) {
        // Going up
        mPlayerToLoad = ATTR_PLAYER_IN1;
      }
      else if (aLevel < mLevel) {
        // Going Down
        mPlayerToLoad = ATTR_PLAYER_IN2;
      }
    }
  }
  else {
    mNextDungeon = aDungeon;
    mPlayerToLoad = ATTR_PLAYER_IN1;
  }

  mNextLevel = aLevel;
  strcpy(mName, gDungeonDefs[mNextDungeon].name);
  mNextTileMapId = gDungeonDefs[mNextDungeon].mInfo.map[aLevel];
  mNextObjectsId = gDungeonDefs[mNextDungeon].mInfo.objectsId;

  if (!mGamePlayfield) {
    mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, mNextTileMapId);
  }
  sprintf(mText, "%s Level %d", mName, aLevel);
}

void GGameState::SetPlayfieldXYFromPlayer(TFloat aPlayerX, TFloat aPlayerY) {
  TFloat maxx = MapWidth(),
         maxy = MapHeight();

  // upper left corner of desired viewport position
  TFloat xx = gViewPort->mWorldX = aPlayerX,
         yy = gViewPort->mWorldY = aPlayerY;

  if (xx < 0) {
    gViewPort->mWorldX = 0;
  }
  else if (xx > maxx) {
    gViewPort->mWorldX = maxx;
  }
  if (yy < 0) {
    gViewPort->mWorldY = 0;
  }
  else if (yy > maxy) {
    gViewPort->mWorldY = maxy;
  }
}

/**
  * This is NOT safe to call from BProcess context
  */
void GGameState::LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId, TBool aNewLevel) {
  mNextGamePlayfield = new GGamePlayfield(gViewPort, mNextTileMapId);

  strcpy(mName, aName);

  const TUint16 overworld_exit = mNextDungeon == OVERWORLD_DUNGEON ? mDungeon : OVERWORLD_DUNGEON;
  const TUint16 exiting_level = mLevel;
  TBool is_same_dungeon = (mDungeon != OVERWORLD_DUNGEON) && (mNextDungeon == mDungeon);

  if (mDungeon == OVERWORLD_DUNGEON) {
    mLastOverworldLevel = mLevel;
  }

  mLevel = mNextLevel = aLevel;
  mDungeon = mNextDungeon;
  mTileMapId = aTileMapId;

  Reset(); // remove sprites and processes
  InitRemapSlots();
  mPlayfield = mGamePlayfield = mNextGamePlayfield;
  gDisplay.SetPalette(mGamePlayfield->GetTilesBitmap(), 0, 128);
  mNextGamePlayfield = ENull;
  GPlayer::mProcess = ENull;

  RemapSlot(mNextObjectsId, ENVIRONMENT_SLOT, IMAGE_32x32);
  RemapSlot(CHARA_HERO_BMP, PLAYER_SLOT);
  RemapSlot(CHARA_HERO_HEAL_EFFECT_BMP, PLAYER_HEAL_SLOT, IMAGE_32x32);
  RemapSlot(CHARA_HERO_SPELL_EFFECT_BMP, PLAYER_SPELL_SLOT, IMAGE_32x32);
  RemapSlot(SPELL_EARTH_BMP, SPELL_EARTH_SLOT, IMAGE_64x64);
  RemapSlot(SPELL_ELECTRICITY_BMP, SPELL_ELECTRICITY_SLOT, IMAGE_64x64);
  RemapSlot(SPELL_FIRE_BMP, SPELL_FIRE_SLOT, IMAGE_64x64);
  RemapSlot(SPELL_WATER_BMP, SPELL_WATER_SLOT, IMAGE_64x64);
  RemapSlot(ENEMY_DEATH_BMP, ENEMY_DEATH_SLOT, IMAGE_32x32);

  GPlayer::mProcess = new GPlayerProcess(this);
  AddProcess(GPlayer::mProcess);

#ifdef DEBUGME
  printf("Level loaded, colors used %d\n", mGamePlayfield->GetTilesBitmap()->CountUsedColors());
#endif

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
#ifdef DEBUGME
    printf("%5d: ", ip);
#endif
    const TUint16 op = program[ip].mCode & TUint32(0xffff),
                  params = program[ip].mCode >> TUint32(16),
                  row = program[ip].mRow,
                  col = program[ip].mCol;

    auto xx = TFloat(col * TILESIZE),
         yy = TFloat(row * TILESIZE);

    switch (op) {

        //
        // ENVIRONMENT
        //

      case ATTR_STONE_STAIRS_UP:
#ifdef DEBUGME
        printf("STONE STAIRS UP at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_UP, "STONE");
        break;

      case ATTR_OW_LEVEL_ENTRANCE:
#ifdef DEBUGME
        printf("OVERWORLD LEVEL TRANSITION at %.2f,%.2f %d,%d %d/%x\n", xx, yy, row, col, params, params);
#endif
        if (mDungeon == OVERWORLD_DUNGEON) {
          GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "DUNGEON");
        }

        break;

      case ATTR_STONE_STAIRS_DOWN:
#ifdef DEBUGME
        printf("STONE STAIRS DOWN at %.2f,%.2f %d,%d %d/%x\n", xx, yy, row, col, params, params);
#endif
        if (mDungeon == OVERWORLD_DUNGEON) {
          GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "DUNGEON");
        }
        else {
          GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "STONE");
        }
        break;

      case ATTR_WOOD_STAIRS_UP:
#ifdef DEBUGME
        printf("WOOD STAIRS UP at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_UP, "WOOD");
        break;

      case ATTR_WOOD_STAIRS_DOWN:
#ifdef DEBUGME
        printf("WOOD STAIRS DOWN at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "WOOD");
        break;

      case ATTR_CRATE:
#ifdef DEBUGME
        printf("CRATE at %.2f,%.2f %d, %d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "CRATE");
        break;

      case ATTR_CRATE_GONE:
#ifdef DEBUGME
        printf("CRATE GONE at %.2f,%.2f %d, %d\n", xx, yy, row, col);
#endif
        if (params) {
          GItemProcess::SpawnItem(this, ip, params, xx, yy + 32);
        }
        break;

      case ATTR_CHEST:
#ifdef DEBUGME
        printf("CHEST CLOSED at %.2f,%.2f %d, %d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "CHEST");
        break;

      case ATTR_CHEST_OPEN:
#ifdef DEBUGME
        printf("CHEST OPEN at %.2f,%.2f %d,%d ATTR: %d\n", xx, yy, row, col, params);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, params, DIRECTION_DOWN, "CHEST");
        break;

      case ATTR_SPIKES:
#ifdef DEBUGME
        printf("SPIKES at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy, spikes_number--, DIRECTION_DOWN, "SPIKES");
        break;

      case ATTR_METAL_DOOR_H:
#ifdef DEBUGME
        printf("METAL DOOR H at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 30, params, DIRECTION_DOWN, "METAL DOOR HORIZONTAL");
        break;

      case ATTR_METAL_DOOR_V:
#ifdef DEBUGME
        printf("METAL DOOR V at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 30, params, DIRECTION_DOWN, "METAL DOOR VERTICAL");
        break;

      case ATTR_WOOD_DOOR_H:
#ifdef DEBUGME
        printf("WOOD DOOR H at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 30, params, DIRECTION_DOWN, "WOOD DOOR HORIZONTAL");
        break;

      case ATTR_WOOD_DOOR_V:
#ifdef DEBUGME
        printf("WOOD DOOR V at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 30, params, DIRECTION_DOWN, "WOOD DOOR VERTICAL");
        break;

      case ATTR_LEVER:
#ifdef DEBUGME
        printf("LEVER at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 30, params, DIRECTION_DOWN, "LEVER");
        break;

      case ATTR_FLOOR_SWITCH:
#ifdef DEBUGME
        printf("FLOOR_SWITCH at %.2f,%.2f %d,%d params: %x\n", xx, yy, row, col, params);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 32, params, DIRECTION_DOWN, "STONE FLOOR SWITCH");
        break;

      case ATTR_FLOOR_SWITCH_WOOD:
#ifdef DEBUGME
        printf("FLOOR_SWITCH at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 32, params, DIRECTION_DOWN, "WOOD FLOOR SWITCH");
        break;

        //
        // PLAYER
        //
      // This case is used for the player 1 entrance for dungeon levels AND overworld entrance auto-detection.
      case ATTR_PLAYER_IN1:
        if (!aNewLevel || mPlayerToLoad != ATTR_PLAYER_IN1) {
          break;
        }
#ifdef DEBUGME
        printf("PLAYER IN1 at %.2f,%.2f\n", xx, yy);
#endif
        if (mDungeon == OVERWORLD_DUNGEON) {
          GPlayer::mProcess->StartLevel(mGamePlayfield, xx, yy, overworld_exit, exiting_level);
          SetPlayfieldXYFromPlayer(xx, yy);
        }
        else {
          GPlayer::mProcess->StartLevel(mGamePlayfield, xx - 16, yy + 28, overworld_exit, exiting_level);
          SetPlayfieldXYFromPlayer(xx - 16, yy + 28);
        }
        startedPlayer = ETrue;
        break;

      // This case is used for the player 2 entrance for levels within dungeons only!
      case ATTR_PLAYER_IN2:
        if (mPlayerToLoad != ATTR_PLAYER_IN2) {
          break;
        }
#ifdef DEBUGME
        printf("PLAYER IN2 at %.2f,%.2f\n", xx, yy);
#endif
        GPlayer::mProcess->StartLevel(mGamePlayfield, xx - 16, yy + 32, overworld_exit, exiting_level);
        SetPlayfieldXYFromPlayer(xx - 16, yy - 32);

        startedPlayer = ETrue;
        break;

      //
      // ENEMIES
      //
      case ATTR_SPIDER:
        RemapSlot(CHARA_SPIDER_BMP, SPIDER_SLOT);

        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("SPIDER at %.2f,%.2f %d,%d\n", xx - 32, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx - 32, yy + 32, params, DIRECTION_DOWN, "ENEMY SPIDER");
        break;

      case ATTR_BAT:
        RemapSlot(CHARA_BAT_BMP, BAT_SLOT);

#ifdef DEBUGME
        printf("BAT at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx - 32, yy + 32, params, DIRECTION_DOWN, "ENEMY BAT");
        break;

      case ATTR_GOBLIN:
        RemapSlot(CHARA_GOBLIN_BMP, GOBLIN_SLOT);
        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("GOBLIN at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 32, params, DIRECTION_DOWN, "ENEMY GOBLIN");
        break;

      case ATTR_GOBLIN_SNIPER:
        RemapSlot(CHARA_GOBLIN_SNIPER_BMP, GOBLIN_SNIPER_SLOT);
        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("GOBLIN_SNIPER at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx - 32, yy + 32, params, DIRECTION_DOWN, "ENEMY GOBLIN SNIPER");
        break;

      case ATTR_ORC:
        RemapSlot(CHARA_ORC_BMP, ORC_SLOT);
        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("ORC at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx, yy + 32, params, DIRECTION_DOWN, "ENEMY ORC");
        break;

      case ATTR_RAT:
        RemapSlot(CHARA_RAT_BMP, RAT_SLOT);

        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("RAT at %.2f,%.2f %d,%d,%d\n", xx, yy, row, col, eCount);
#endif
        GProcess::Spawn(this, op, ip, xx - 18, yy + 32, params, DIRECTION_DOWN, "ENEMY RAT");
        break;

      case ATTR_SLIME:
        RemapSlot(CHARA_SLIME_BMP, SLIME_SLOT);

        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("SLIME at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx - 18, yy + 31, params, DIRECTION_DOWN, "ENEMY SLIME");
        break;

      case ATTR_TROLL:
        RemapSlot(CHARA_TROLL_BMP, TROLL_SLOT);

        if (!aNewLevel) {
          break;
        }
#ifdef DEBUGME
        printf("TROLL at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        GProcess::Spawn(this, op, ip, xx - 20, yy + 32, params, DIRECTION_DOWN, "ENEMY TROLL");
        break;

        // mid boss
        // only one mid boss can be available
      case ATTR_MID_BOSS_ENERGY:
#ifdef DEBUGME
        printf("MID BOSS ENERGY at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_FIRE_BMP, BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_ENERGY_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY MID BOSS ENERGY");
        break;

      case ATTR_MID_BOSS_FIRE:
#ifdef DEBUGME
        printf("MID BOSS FIRE at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_FIRE_BMP, BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_FIRE_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY MID BOSS FIRE");
        break;

      case ATTR_MID_BOSS_EARTH:
#ifdef DEBUGME
        printf("MID BOSS EARTH at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_EARTH_BROWN_BMP, BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_EARTH_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY MID BOSS EARTH");
        break;

      case ATTR_MID_BOSS_WATER:
#ifdef DEBUGME
        printf("MID BOSS WATER at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_WATER_BMP, BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_WATER_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY MID BOSS WATER");
        break;

      case ATTR_WIZARD_EARTH:
#ifdef DEBUGME
        printf("EARTH WIZARD at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(EARTH_WIZARD_BMP, BOSS_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy projectiles
        RemapSlot(EARTH_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        RemapSlot(EARTH_PILLAR_BMP, BOSS_PILLAR_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY EARTH WIZARD");
        break;

      case ATTR_WIZARD_WATER:
#ifdef DEBUGME
        printf("WATER WIZARD at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(WATER_WIZARD_BMP, BOSS_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy projectiles
        RemapSlot(WATER_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        RemapSlot(WATER_PILLAR_BMP, BOSS_PILLAR_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY WATER WIZARD");
        break;

      case ATTR_WIZARD_FIRE:
#ifdef DEBUGME
        printf("FIRE WIZARD at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(FIRE_WIZARD_BMP, BOSS_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy projectiles
        RemapSlot(FIRE_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        RemapSlot(FIRE_PILLAR_BMP, BOSS_PILLAR_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY FIRE WIZARD");
        break;

      case ATTR_WIZARD_ENERGY:
#ifdef DEBUGME
        printf("ENERGY WIZARD at %.2f,%.2f %d,%d\n", xx, yy, row, col);
#endif
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(ENERGY_WIZARD_BMP, BOSS_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy projectiles
        RemapSlot(ENERGY_PROJECTILE_BMP, BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        RemapSlot(ENERGY_PILLAR_BMP, BOSS_PILLAR_SLOT, IMAGE_32x32);
        if (!aNewLevel) {
          break;
        }
        GProcess::Spawn(this, op, ip, xx, yy + 64, params, DIRECTION_DOWN, "ENEMY ENERGY WIZARD");
        break;

      default:
        printf("======== > Invalid op code in Object Program: $%0x at col,row %d,%d\n", program[ip].mCode, col, row);
        break;
    }
  }
  if (!startedPlayer) {
    printf("NO PLAYER at %.2f,%.2f\n", 32., 64.);
    GPlayer::mProcess->StartLevel(mGamePlayfield, 32. + 32, 64. + 63);
  }
  mGamePlayfield->StartMosaicIn();
}

/**
  * modify BObjectProgram, like when a door or chest has opened.
  *
  * aCode is the code to change the program step to, OR ATTR_KEEP to leave the code in the BObjectProgram unchanged.
  * aAttr is the attribute to change the program step to, OR ATTR_KEEP
  */
void GGameState::EndProgram(TInt aIp, TUint16 aCode, TUint16 aAttr) {
  BObjectProgram *program = mGamePlayfield->mObjectProgram,
                 *step = &program[aIp];

  TUint32 code = aCode,
          attr = aAttr,
          sCode = step->mCode;

  if (aCode == ATTR_KEEP) {
    if (aAttr == ATTR_KEEP) {
      // keep both?  Nothing to do!
      return;
    }
    step->mCode = LOWORD(sCode) | (attr << 16);
  }
  else if (aAttr == ATTR_KEEP) {
    step->mCode = (sCode & 0xffff0000) | (attr << 16);
  }
  else {
    TUint32 new_code = code | (attr << 16);
    step->mCode = new_code;
  }
  printf("EndProgram %p (%08x -> %08x)\n", program, sCode, step->mCode);
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::GameOver() {
  mGameOver = new GGameOver(this);
  gControls.Reset();
  GPlayer::mGameOver = ETrue;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::InitRemapSlots() {
  for (TInt i = 2; i < SLOT_MAX; i++) {
    if (i == TILESET_SLOT) {
      // do not release TILESET
      continue;
    }
    if (mSlotRemapState[i]) {
      if (gResourceManager.GetBitmap(i)) {
        printf("Releasing slot %d\n", i);
        gResourceManager.ReleaseBitmapSlot(i);
      }
    }
    mSlotRemapState[i] = EFalse;
  }
}

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot, TInt16 aImageSize) {
  if (aSlot == TILESET_SLOT) {
    Panic("Attemp to RemapSlot(TILESET_SLOT)\n");
  }
  if (mSlotRemapState[aSlot]) {
    return;
  }

  gResourceManager.LoadBitmap(aBMP, aSlot, aImageSize);

  BBitmap *screen = mGamePlayfield->GetTilesBitmap();
  BBitmap *bm = gResourceManager.GetBitmap(aSlot);
  if (!mSlotRemapState[aSlot]) {
    bm->Remap(screen);
    mSlotRemapState[aSlot] = ETrue;
#ifdef DEBUGME
    printf("Remapped bitmap %d to slot %d, screen colors used %d\n", aBMP, aSlot, screen->CountUsedColors());
#endif
  }
  gDisplay.SetPalette(screen->GetPalette());
}

TBool GGameState::SaveState() {
  printf("\n======= BEGIN %s =======\n", __FUNCTION__);
  BMemoryStream stream;

  TUint32 seed = GetRandomSeed();
  stream.Write(&seed, sizeof(TUint32));

  printf("Write BMapPlayfield\n");
  stream.PrintMSize();
  BMapPlayfield::WriteToStream(&stream, NUM_RESOURCES);
  stream.Write(&mTileMapId, sizeof(mTileMapId));
  stream.Write(&mNextDungeon, sizeof(mNextDungeon));
  stream.Write(&mLevel, sizeof(mLevel));
  stream.Write(&mDungeon, sizeof(mDungeon));
  stream.Write(&mLastOverworldLevel, sizeof(mLastOverworldLevel));
  stream.PrintMSize();

  //  mGamePlayfield->Restore();
  printf("Writing Player\n");
  stream.PrintMSize();
  GPlayer::WriteToStream(stream);
  stream.PrintMSize();

  // walk through process list and save enemies states
  for (GProcess *p = (GProcess *)mProcessList.First(); !mProcessList.End(p); p = (GProcess *)mProcessList.Next(p)) {
    if (p->mAttribute != ATTR_GONE && p->mAttribute != ATTR_PLAYER_IN1 && p->mAttribute != ATTR_PLAYER_IN2) {
      if (p->mSaveToStream) {
#ifndef __DINGUX__
        printf("Writing attribute %i (%s)\n", p->mAttribute, typeid(*p).name());
#endif
        stream.PrintMSize();
        stream.Write(&p->mAttribute, sizeof(p->mAttribute));
        stream.PrintMSize();

#ifndef __DINGUX__
        printf("Writing state for %s\n", typeid(*p).name());
#endif
        stream.PrintMSize();
        p->WriteToStream(stream);
        stream.PrintMSize();

        //        p->WriteToStream(stream);
      }
#ifndef __DINGUX__
      else {
        printf("Skipping %i %s\n", p->mAttribute, typeid(*p).name());
      }
#endif
    }
  }

  // write attribute of -1 to signify end of list in the stream
  TInt16 attr = -1;
  stream.Write(&attr, sizeof(attr));

  printf("Writing Player mSprite\n");
  stream.PrintMSize();
  GAnchorSprite *s = GPlayer::mSprite;
  s->WriteToStream(stream);
  stream.PrintMSize();

  printf("SAVE Stream size %i\n", stream.Size());
  gSavedGameList.SaveGame(stream.Data(), stream.Size(), mText);
  mTimer = FRAMES_PER_SECOND * 1;
  printf("\n-------- END %s--------\n", __FUNCTION__);

  return ETrue;
}

TBool GGameState::LoadState(const char *aGameName) {

  printf("\n======= BEGIN %s =======\n", __FUNCTION__);
  BMemoryStream stream = *gSavedGameList.LoadSavedGame(aGameName);
  printf("LOAD Stream size %i\n", stream.Size());

  TUint32 seed;
  stream.Read(&seed, sizeof(TUint32));
  SeedRandom(seed);

  printf("Reading BMapPlayfield\n");
  stream.PrintReadIndex();
  BMapPlayfield::ReadFromStream(&stream, NUM_RESOURCES);
  stream.Read(&mTileMapId, sizeof(mTileMapId));
  stream.Read(&mNextDungeon, sizeof(mNextDungeon));
  stream.Read(&mLevel, sizeof(mLevel));
  stream.Read(&mDungeon, sizeof(mDungeon));
  stream.Read(&mLastOverworldLevel, sizeof(mLastOverworldLevel));
  stream.PrintReadIndex();

  // spawn it all
  NextLevel(mNextDungeon, mLevel);
  LoadLevel(mName, mLevel, mNextTileMapId, EFalse);

  //  mGamePlayfield->DumpObjectProgram();
  printf("Reading Player\n");
  stream.PrintReadIndex();
  GPlayer::ReadFromStream(stream);
  stream.PrintReadIndex();

  GPlayer::mProcess->StartLevel(mGamePlayfield, GPlayer::mSprite->x, GPlayer::mSprite->y);

  printf("Reading all processes\n");

  TInt16 attr = 0;
  while (attr != -1) {
    printf("Read an attribute\n");
    stream.PrintReadIndex();
    stream.Read(&attr, sizeof(attr));
    stream.PrintReadIndex();
    printf("Attribute found: %i\n", attr);

    if (attr != -1) {

      GProcess *p = GProcess::Spawn(this, attr, 0, 0, 0, 0, DIRECTION_DOWN);
      if (!p) {
        Panic("GGameState::LoadState Did not spawn process for attribute %i\n", attr);
      }
      printf("Reading stream for process %i\n", attr);
      stream.PrintReadIndex();
      p->ReadFromStream(stream);
      stream.PrintReadIndex();
      //      p->mSprite->Dump();
    }
  }

  printf("Reading Player Sprite\n");
  stream.PrintReadIndex();
  GAnchorSprite *s = GPlayer::mSprite;
  s->ReadFromStream(stream);
  stream.PrintReadIndex();

  printf("\n-------- END %s--------\n", __FUNCTION__);
  return ETrue;
}
