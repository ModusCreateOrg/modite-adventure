#include <Display.h>
#include <BViewPort.h>
#include "GResources.h"
#include "BResourceManager.h"
#include "GPlayer.h"
#include "GHud.h"
#include "Items.h"

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

static void render_meter(BViewPort *vp, BBitmap *screen, TUint8 color, TInt x, TInt y, TFloat value, TFloat max) {
  TFloat pct = (value / max);

  const TInt innerWidth = METER_WIDTH - 2;

  TInt w = (TInt)(pct *  innerWidth);

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

void GHud::SetColors() {

  printf("GHud::SetColors()\n");
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
  render_meter(&vp, screen, COLOR_HEALTH, 26, 2, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);

  screen->DrawBitmapTransparent(&vp, b, magic, 91, 0);
  render_meter(&vp, screen, COLOR_MAGIC, 105, 2, GPlayer::mManaPotion, 100);

  screen->DrawBitmapTransparent(&vp, b, sword, 168, 0);
  render_meter(&vp, screen, COLOR_EXPERIENCE, 169 + 20, 2, GPlayer::mExperience, GPlayer::mNextLevel);

  const GInventoryItem *item = GPlayer::mEquipped.mSpellbook;
  if (item) {
    switch (item->mItemNumber) {
      case ITEM_BLUE_SPELLBOOK:
        screen->DrawBitmapTransparent(&vp, b, water, 249, 0);
        break;
      case ITEM_YELLOW_SPELLBOOK:
        screen->DrawBitmapTransparent(&vp, b, energy, 249, 0);
        break;
      case ITEM_RED_SPELLBOOK:
        screen->DrawBitmapTransparent(&vp, b, fire, 249, 0);
        break;
      case ITEM_GREEN_SPELLBOOK:
        screen->DrawBitmapTransparent(&vp, b, earth, 249, 0);
        break;
    }
  }
  if (GPlayer::mInventoryList.FindItem(ITEM_SILVER_KEY)) {
    screen->DrawBitmapTransparent(&vp, b, silver_key, 278, 0);
  }
  if (GPlayer::mInventoryList.FindItem(ITEM_GOLD_KEY)) {
    screen->DrawBitmapTransparent(&vp, b, gold_key, 294, 0);
  }
}
