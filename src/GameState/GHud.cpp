#include <Display.h>
#include <BViewPort.h>
#include "GResources.h"
#include "BResourceManager.h"
#include "GPlayer.h"
#include "GHud.h"
#include "Items.h"

static const TRect heart(0, 416, 15, 431);
static const TRect magic(16, 416, 31, 431);
static const TRect sword(0, 432, 15, 463);

static const TRect water(32, 416, 32, 431);
static const TRect fire(48, 416, 48 + 15, 416 + 15);
static const TRect earth(32, 416 + 16, 32 + 15, 416 + 31);
static const TRect energy(48, 416 + 16, 48 + 15, 416 + 31);

// TODO: @jaygarcia no silver or godld key images on GLOBAL_OBJECT_LAYER.bmp
// TODO: add them and then adjust these rects
static const TRect silver_key(48, 416 + 16, 48 + 15, 416 + 31);
static const TRect gold_key(48, 416 + 16, 48 + 15, 416 + 31);

// meter width:
// 26, 70
static const TInt METER_WIDTH = 44;
static const TInt METER_HEIGHT = 11;

static void render_meter(BViewPort *vp, BBitmap *screen, TUint8 color, TInt x, TInt y, TFloat value, TFloat max) {
  TInt w = value ? value / max * 42 : 0;

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

void GHud::Render() {
  BBitmap *b = gResourceManager.GetBitmap(ENVIRONMENT_SLOT),
          *screen = gDisplay.renderBitmap;

  BViewPort vp;
  TRect rect(0, 0, SCREEN_WIDTH - 1, 15);
  vp.SetRect(rect);
  gDisplay.renderBitmap->FillRect(&vp, vp.mRect, COLOR_HUD_BG);

  screen->SetColor(COLOR_HUD_BG, 0, 0, 0);
  screen->SetColor(COLOR_METER_OUTLINE, 64, 64, 64);     // outline color for meter
  screen->SetColor(COLOR_HEALTH, 0xf9, 0xa4, 0xa1);      // light
  screen->SetColor(COLOR_HEALTH2, 0xff, 0x59, 0x43);     // dark
  screen->SetColor(COLOR_MAGIC, 0x63, 0xab, 0xf1);       // light
  screen->SetColor(COLOR_MAGIC2, 0x43, 0x7b, 0xf0);      // dark
  screen->SetColor(COLOR_EXPERIENCE, 0x6c, 0xd8, 0x20);  // light
  screen->SetColor(COLOR_EXPERIENCE2, 0x2d, 0xa1, 0x2f); // dark

  screen->DrawBitmapTransparent(&vp, b, heart, 8, 0);
  render_meter(&vp, screen, COLOR_HEALTH, 26, 3, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);

  screen->DrawBitmapTransparent(&vp, b, magic, 91, 0);
  render_meter(&vp, screen, COLOR_MAGIC, 105, 3, GPlayer::mManaPotion, 100);

  screen->DrawBitmapTransparent(&vp, b, sword, 168, 0);
  render_meter(&vp, screen, COLOR_EXPERIENCE, 169 + 20, 3, GPlayer::mExperience, GPlayer::mNextLevel);

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
