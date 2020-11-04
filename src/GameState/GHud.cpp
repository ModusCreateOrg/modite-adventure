#include <Display.h>
#include <BViewPort.h>
#include "GResources.h"
#include "BResourceManager.h"
#include "GPlayer.h"
#include "GHud.h"
#include "Items.h"
#include "Game.h"
#include "GBossProcess.h"

static const TRect heart(0, 384, 15, 399);
static const TRect magic(16, 384, 31, 399);
static const TRect sword(0, 400, 15, 415);

static const TRect water(32, 384, 47, 399);
static const TRect fire(48, 384, 48 + 15, 399);
static const TRect earth(32, 400, 32 + 15, 415);
static const TRect energy(48, 400, 48 + 15, 415);


static const TRect silver_key(64, 384, 79,399);
static const TRect gold_key(64, 400, 79, 415);

// meter width:
// 26, 70
static const TInt METER_WIDTH = 60;
static const TInt METER_HEIGHT = 12;

static const TInt STAT_WIDTH = 200;

static void render_meter(BViewPort *vp, BBitmap *screen, TUint8 color, TInt x, TInt y, TFloat value, TFloat max) {
  TFloat pct = (value / max);

  const TInt innerWidth = METER_WIDTH - 2;

  TInt w = (TInt)(pct *  innerWidth);
  if (w < 2) {
    w = 2;
  }

  screen->DrawRect(vp, x, y, x + METER_WIDTH - 1, y + METER_HEIGHT - 1, COLOR_METER_OUTLINE);
  screen->DrawFastHLine(vp, x + 1, y + 1, w, color);
  screen->FillRect(vp, x + 1, y + 2, x + w, y + METER_HEIGHT - 2, TUint8(color + 1));

  // erase the corners
  screen->DrawFastHLine(vp, x, y, 2, COLOR_HUD_BG);
  screen->DrawFastVLine(vp, x, y, 2, COLOR_HUD_BG);

  screen->DrawFastHLine(vp, x + METER_WIDTH - 2, y, 2, COLOR_HUD_BG);
  screen->DrawFastVLine(vp, x + METER_WIDTH - 1, y, 2, COLOR_HUD_BG);

  screen->DrawFastHLine(vp, x, y + METER_HEIGHT - 1, 2, COLOR_HUD_BG);
  screen->DrawFastVLine(vp, x, y + METER_HEIGHT - 2, 2, COLOR_HUD_BG);

  screen->DrawFastHLine(vp, x + METER_WIDTH - 2, y + METER_HEIGHT - 1, 2, COLOR_HUD_BG);
  screen->DrawFastVLine(vp, x + METER_WIDTH - 1, y + METER_HEIGHT - 2, 2, COLOR_HUD_BG);

  // draw the 4 pixels to round off the oval
  screen->WritePixel(x + 1, y + 1, COLOR_METER_OUTLINE);
  screen->WritePixel(x + METER_WIDTH - 2, y + 1, COLOR_METER_OUTLINE);

  screen->WritePixel(x + 1, y + METER_HEIGHT - 2, COLOR_METER_OUTLINE);
  screen->WritePixel(x + METER_WIDTH - 2, y + METER_HEIGHT - 2, COLOR_METER_OUTLINE);
}


static TUint32 frame = 0;
static TBool darkHealth = EFalse;
void GHud::SetColors() {
  frame = 0;

//  printf("GHud::SetColors()\n");
  gDisplay.renderBitmap->SetColor(COLOR_HUD_BG, 0, 0, 0);
  gDisplay.renderBitmap->SetColor(COLOR_METER_OUTLINE, 64, 64, 64);     // outline color for meter
  gDisplay.renderBitmap->SetColor(COLOR_HEALTH, 0xf9, 0xa4, 0xa1);      // light
  gDisplay.renderBitmap->SetColor(COLOR_HEALTH2, 0xff, 0x59, 0x43);     // dark
  gDisplay.renderBitmap->SetColor(COLOR_MAGIC, 0x63, 0xab, 0xf1);       // light
  gDisplay.renderBitmap->SetColor(COLOR_MAGIC2, 0x43, 0x7b, 0xf0);      // dark
  gDisplay.renderBitmap->SetColor(COLOR_EXPERIENCE, 0x6c, 0xd8, 0x20);  // light
  gDisplay.renderBitmap->SetColor(COLOR_EXPERIENCE2, 0x2d, 0xa1, 0x2f); // dark
}



void GHud::Render() {
  BBitmap *b = gResourceManager.GetBitmap(ENVIRONMENT_SLOT),
          *screen = gDisplay.renderBitmap;

  BViewPort vp;
  TRect rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);
  gDisplay.renderBitmap->FillRect(&vp, vp.mRect, COLOR_HUD_BG);


  screen->DrawBitmapTransparent(&vp, b, heart, 8, 0);

  TFloat healthPct = (TFloat)GPlayer::mHitPoints / (TFloat)GPlayer::mMaxHitPoints;
  frame++;

  if (healthPct < .10f && frame % 30 == 0)  {
    darkHealth = ! darkHealth;
    if (darkHealth) {
      gSoundPlayer.TriggerSfx(SFX_PLAYER_LOW_HEALTH_WAV, 2);
    }

  }

  if (darkHealth && healthPct > .10f) {
    darkHealth = EFalse;
  }

  if (darkHealth) {
    gDisplay.renderBitmap->SetColor(COLOR_HEALTH, 0xf9 - 50, 0xa4 - 50, 0xa1 - 50);
    gDisplay.renderBitmap->SetColor(COLOR_HEALTH2, 0xff - 50, 0x59 - 50, 0x43 - 50);     // dark
  }
  else{
    gDisplay.renderBitmap->SetColor(COLOR_HEALTH, 0xf9, 0xa4, 0xa1);
    gDisplay.renderBitmap->SetColor(COLOR_HEALTH2, 0xff, 0x59, 0x43);     // dark
  }

  render_meter(&vp, screen, COLOR_HEALTH, 26, 2, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);

  screen->DrawBitmapTransparent(&vp, b, magic, 91, 0);
  render_meter(&vp, screen, COLOR_MAGIC, 105, 2, GPlayer::mManaPotion, GPlayer::mMaxMana);

  screen->DrawBitmapTransparent(&vp, b, sword, 168, 0);
  render_meter(&vp, screen, COLOR_EXPERIENCE, 169 + 20, 2, GPlayer::mExperience, GPlayer::mNextLevel);

  switch (GPlayer::mEquipped.mSpellBookElement) {
    case ELEMENT_WATER:
      screen->DrawBitmapTransparent(&vp, b, water, 249, 0);
      break;
    case ELEMENT_FIRE:
      screen->DrawBitmapTransparent(&vp, b, energy, 249, 0);
      break;
    case ELEMENT_ENERGY:
      screen->DrawBitmapTransparent(&vp, b, fire, 249, 0);
      break;
    case ELEMENT_EARTH:
      screen->DrawBitmapTransparent(&vp, b, earth, 249, 0);
      break;
    default:
      break;
  }
  if (GPlayer::mInventoryList.FindItem(ITEM_SILVER_KEY)) {
    screen->DrawBitmapTransparent(&vp, b, silver_key, 278, 0);
  }
  if (GPlayer::mInventoryList.FindItem(ITEM_GOLD_KEY)) {
    screen->DrawBitmapTransparent(&vp, b, gold_key, 294, 0);
  }

  GBossProcess *boss = GPlayer::mActiveBoss;
  if (boss) {
    TInt h = gViewPort->mRect.Height();
    for (TInt i = 1; i <= boss->mHealthBarCount; i++) {
      gDisplay.renderBitmap->DrawFastHLine(gViewPort, 21, h - 11, STAT_WIDTH + 1, COLOR_METER_OUTLINE);
      gDisplay.renderBitmap->DrawFastVLine(gViewPort, 22 + STAT_WIDTH, h - 15, 5, COLOR_METER_OUTLINE);
      gDisplay.renderBitmap->FillRect(gViewPort, 20, h - 16, 21 + STAT_WIDTH, h - 12, COLOR_TEXT);
      if (boss->mCurrentHealthBar == i) {
        if (boss->mHitPoints > 0) {
          gDisplay.renderBitmap->FillRect(gViewPort, 20, h - 16, 21 + boss->mHitPoints * STAT_WIDTH / boss->mMaxHitPoints,
                                          h - 12, COLOR_HEALTH2);
        }
      } else if (boss->mCurrentHealthBar > i) {
        gDisplay.renderBitmap->FillRect(gViewPort, 20, h - 16, 21 + STAT_WIDTH, h - 12, COLOR_HEALTH2);
      }
      h -= 7;
    }
    gDisplay.renderBitmap->DrawStringShadow(gViewPort, boss->mSprite->Name(), gFont8x8, 21, h - 18, COLOR_TEXT,
                                            COLOR_TEXT_SHADOW,
                                            COLOR_TEXT_TRANSPARENT);
  }
}
