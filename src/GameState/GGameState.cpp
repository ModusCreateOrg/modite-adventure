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
#include "GameState/mid-bosses/GMidBossGenericProcess.h"

#include "GPlayer.h"

#define DEBUGME
//#undef DEBUGME

const TInt GAUGE_WIDTH = 90;

// info about the dungeons
static struct DUNGEON_DEF {
  const char *name;
  TUint16 map[10];
} dungeon_defs[] = {
  // DUNGEON_DEV
  { "DEV DUNGEON",
      {
          DEVDUNGEON_0_LEVEL1_MAP,
          DEVDUNGEON_0_LEVEL1_MAP,
          DEVDUNGEON_0_LEVEL2_MAP,
          DEVDUNGEON_0_LEVEL3_MAP,
          DEVDUNGEON_0_LEVEL4_MAP,
      } },
};
const TInt NUM_DUNGEONS = sizeof(dungeon_defs) / sizeof(DUNGEON_DEF);

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

  mGamePlayfield = mPreviousPlayfield = ENull;
  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  GPlayer::Init();
  NextLevel(DUNGEON_DEV, 2);
}

GGameState::~GGameState() { gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT); }

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::PreRender() {
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
  if (mText[0]) {
    TInt len = strlen(mText);
    TInt x = gViewPort->mRect.Width() / 2 - len * 12 / 2;
    gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);
    gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont16x16, x, 32, COLOR_SHMOO_GREY, COLOR_TEXT_TRANSPARENT, -4);
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
  for (TInt s = 0; s < 16; s++) {
    mGamePlayfield->mGroupState[s] = ETrue;
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
 * @param aDungeon  ID of dungeon (in dungeon_defs)
 * @param aLevel    Level in dungeon
 */
void GGameState::NextLevel(const TInt16 aDungeon, const TInt16 aLevel) {
  mNextDungeon = aDungeon;
  mNextLevel = aLevel;
  strcpy(mName, dungeon_defs[aDungeon].name);
  mNextTileMapId = dungeon_defs[aDungeon].map[aLevel];

  mPreviousPlayfield = ENull;
  mPlayfield = ENull;
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, mNextTileMapId);
  sprintf(mText, "%s Level %d", mName, aLevel);
  mTimer = 1 * FRAMES_PER_SECOND;
  Disable();
}

void GGameState::LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId) {
  strcpy(mName, aName);
  mLevel = mNextLevel = aLevel;
  mTileMapId = aTileMapId;

  delete mPreviousPlayfield;
  Reset(); // remove sprites and processes
  GPlayer::mProcess = ENull;
  for (TBool &i : slotRemapState) {
    i = EFalse;
  }

  //  delete mPlayfield;
  //
  //  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, aTileMapId);
  //  sprintf(mText, "%s Level %d", aName, aLevel);
  //  mTimer = 1 * FRAMES_PER_SECOND;
  //  Disable();

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

  printf("Level loaded, colors used %d\n", mGamePlayfield->GetTilesBitmap()->CountUsedColors());

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
    const TUint16 op = program[ip].mCode & TUint32(0xffff),
                  params = program[ip].mCode >> TUint32(16),
                  row = program[ip].mRow,
                  col = program[ip].mCol;

    auto xx = TFloat(col * 32), yy = TFloat(row * 32);

    switch (op) {

        //
        // ENVIRONMENT
        //

      case ATTR_STONE_STAIRS_UP:
#ifdef DEBUGME
        printf("STONE STAIRS UP at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GStairsProcess(this, ip, DIRECTION_UP, params, xx, yy, "STONE"));
        break;

      case ATTR_STONE_STAIRS_DOWN:
#ifdef DEBUGME
        printf("STONE STAIRS DOWN at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GStairsProcess(this, ip, DIRECTION_DOWN, params, xx, yy, "STONE"));
        break;

      case ATTR_WOOD_STAIRS_UP:
#ifdef DEBUGME
        printf("WOOD STAIRS UP at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GStairsProcess(this, ip, DIRECTION_UP, params, xx, yy, "WOOD"));
        break;

      case ATTR_WOOD_STAIRS_DOWN:
#ifdef DEBUGME
        printf("WOOD STAIRS DOWN at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GStairsProcess(this, ip, DIRECTION_DOWN, params, xx, yy, "WOOD"));
        break;

      case ATTR_CRATE:
#ifdef DEBUGME
        printf("CRATE at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GCrateProcess(this, ip, params, xx, yy));
        break;

      case ATTR_CRATE_GONE:
#ifdef DEBUGME
        printf("CRATE GONE at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        if (params) {
          GItemProcess::SpawnItem(this, ip, params, xx, yy + 32);
        }
        break;

      case ATTR_CHEST:
#ifdef DEBUGME
        printf("CHEST CLOSED at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GChestProcess(this, ip, params, xx, yy, EFalse));
        break;

      case ATTR_CHEST_OPEN:
#ifdef DEBUGME
        printf("CHEST OPEN at %.2f,%.2f %d %d ATTR: %d\n", xx, yy, row, col, params);
#endif
        AddProcess(new GChestProcess(this, ip, params, xx, yy, ETrue));
        break;

      case ATTR_SPIKES:
#ifdef DEBUGME
        printf("SPIKES at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GSpikesProcess(this, ip, xx, yy + 30, spikes_number--));
        break;

      case ATTR_METAL_DOOR_H:
#ifdef DEBUGME
        printf("METAL DOOR H at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GDoorProcess(this, ip, params, xx, yy + 30, EFalse, ETrue));
        break;

      case ATTR_METAL_DOOR_V:
#ifdef DEBUGME
        printf("METAL DOOR V at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GDoorProcess(this, ip, params, xx, yy + 30, EFalse, EFalse));
        break;

      case ATTR_WOOD_DOOR_H:
#ifdef DEBUGME
        printf("WOOD DOOR H at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GDoorProcess(this, ip, params, xx, yy + 30, ETrue, ETrue));
        break;

      case ATTR_WOOD_DOOR_V:
#ifdef DEBUGME
        printf("WOOD DOOR V at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GDoorProcess(this, ip, params, xx, yy + 30, ETrue, EFalse));
        break;

      case ATTR_LEVER:
#ifdef DEBUGME
        printf("LEVER at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GLeverProcess(this, ip, params, xx, yy + 32));
        break;

      case ATTR_FLOOR_SWITCH:
#ifdef DEBUGME
        printf("FLOOR_SWITCH at %.2f,%.2f %d %d params: %x\n", xx, yy, row, col, params);
#endif
        AddProcess(new GFloorSwitchProcess(this, ip, params, xx, yy + 32, EFalse));
        break;

      case ATTR_FLOOR_SWITCH_WOOD:
#ifdef DEBUGME
        printf("FLOOR_SWITCH at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GFloorSwitchProcess(this, ip, params, xx, yy + 32, ETrue));
        break;

        //
        // PLAYER
        //

      case ATTR_PLAYER:
#ifdef DEBUGME
        printf("PLAYER at %.2f,%.2f\n", xx, yy);
#endif
        GPlayer::mProcess->StartLevel(mGamePlayfield, xx - 16, yy + 32);
        startedPlayer = ETrue;
        break;

        //
        // ENEMIES
        //

      case ATTR_SPIDER:
#ifdef DEBUGME
        printf("SPIDER at %.2f,%.2f %d %d\n", xx - 32, yy, row, col);
#endif
        AddProcess(new GSpiderProcess(this, ip, xx - 32, yy + 32, params));
        break;

      case ATTR_BAT:
#ifdef DEBUGME
        printf("BAT at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GBatProcess(this, ip, xx - 32, yy + 32, params));
        break;

      case ATTR_GOBLIN:
#ifdef DEBUGME
        printf("GOBLIN at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GGoblinProcess(this, ip, xx, yy + 32, params));
        break;

      case ATTR_GOBLIN_SNIPER:
#ifdef DEBUGME
        printf("GOBLIN_SNIPER at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GGoblinSniperProcess(this, ip, xx - 32, yy + 32, params));
        break;

      case ATTR_ORC:
#ifdef DEBUGME
        // TODO @jaygarcia Using our test level 1, we spawn 2+ ORCs
        // to test other enemy logic, comment out spawning the Orc and instead spawn the enemy we want to see/test
        printf("ORC at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        //        AddProcess(new GGoblinProcess(this, xx, yy + 32, params));
        AddProcess(new GOrcProcess(this, ip, xx, yy + 32, params));
        break;

      case ATTR_RAT:
#ifdef DEBUGME
        printf("RAT at %.2f,%.2f %d %d %d\n", xx, yy, row, col, eCount);
#endif
        AddProcess(new GRatProcess(this, ip, xx - 18, yy + 31, params));
        break;

      case ATTR_SLIME:
#ifdef DEBUGME
        printf("SLIME at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GSlimeProcess(this, ip, xx - 18, yy + 31, params));
        break;

      case ATTR_TROLL:
#ifdef DEBUGME
        printf("TROLL at %.2f,%.2f %d %d\n", xx, yy, row, col);
#endif
        AddProcess(new GTrollProcess(this, ip, xx - 20, yy + 32, params));
        break;

        //const TUint16 ATTR_MID_BOSS_ENERGY = 1001;
        //const TUint16 ATTR_MID_BOSS_FIRE = 1002;
        //const TUint16 ATTR_MID_BOSS_EARTH= 1003;
        //const TUint16 ATTR_MID_BOSS_WATER= 1004;
        // mid boss
        // only one mid boss can be available
      case ATTR_MID_BOSS_ENERGY:
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, MID_BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_ENERGY_BMP, MID_BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_ENERGY_PROJECTILE_BMP, MID_BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        AddProcess(new GMidBossGenericProcess(this, xx, yy + 64, MID_BOSS_SLOT));
        break;
      case ATTR_MID_BOSS_FIRE:
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, MID_BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_FIRE_BMP, MID_BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_FIRE_PROJECTILE_BMP, MID_BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        AddProcess(new GMidBossGenericProcess(this, xx, yy + 64, MID_BOSS_SLOT));
        break;

      case ATTR_MID_BOSS_EARTH:
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, MID_BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_EARTH_BROWN_BMP, MID_BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_EARTH_PROJECTILE_BMP, MID_BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        AddProcess(new GMidBossGenericProcess(this, xx, yy + 64, MID_BOSS_SLOT));
        break;

      case ATTR_MID_BOSS_WATER:
        // always explosion (for any enemy)
        RemapSlot(MID_BOSS_DEATH_EXPLOSION_BMP, MID_BOSS_DEATH_SLOT, IMAGE_64x64);
        // Sprite sheet for enemy
        RemapSlot(MID_BOSS_WATER_BMP, MID_BOSS_SLOT, IMAGE_128x128);
        // Sprite sheet for enemy projectiles
        RemapSlot(MID_BOSS_WATER_PROJECTILE_BMP, MID_BOSS_PROJECTILE_SLOT, IMAGE_32x32);
        AddProcess(new GMidBossGenericProcess(this, xx, yy + 64, MID_BOSS_SLOT));
        break;

      default:
        printf("Invalid op code in Object Program: $%0x at col,row %d,%d\n", program[ip].mCode, col, row);
        break;
    }
  }
  if (!startedPlayer) {
    printf("NO PLAYER at %.2f,%.2f\n", 32., 64.);
    GPlayer::mProcess->StartLevel(mGamePlayfield, 32. + 32, 64. + 63);
  }
}

void GGameState::EndProgram(TInt aIp, TUint16 aCode, TUint16 aAttr) {
  BObjectProgram *program = mGamePlayfield->mObjectProgram,
                 *step = &program[aIp];

  printf("EndProgram %p]n", program);
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
    step->mCode = code | (attr << 16);
  }
}
