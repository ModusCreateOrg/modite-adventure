#include <BSpriteSheet.h>
#include "GInventoryPlayfield.h"
#include "GPlayer.h"
#include "Items.h"

static const TInt METER_WIDTH = 100;
static const TInt METER_HEIGHT = 12;

static void render_meter(BViewPort *vp, const char *name, TUint8 color, TInt x, TInt y, TInt value, TInt max) {
  BBitmap *bm = gDisplay.renderBitmap;
  bm->DrawString(vp, name, gFont8x8, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  x += 56, y -= 2;

  TFloat pct = TFloat(value) / max;

  const TInt innerWidth = METER_WIDTH - 2;

  TInt w = (TInt)(pct *  innerWidth);

  bm->DrawRect(vp, x, y, x + METER_WIDTH - 1, y + METER_HEIGHT - 1, COLOR_METER_OUTLINE);
  bm->DrawFastHLine(vp, x + 1, y + 1, w, color);
  bm->FillRect(vp, x + 1, y + 2, x + w, y + METER_HEIGHT - 2, TUint8(color + 1));

  // erase the corners
  bm->DrawFastHLine(vp, x, y, 2, COLOR_TEXT_BG);
  bm->DrawFastVLine(vp, x, y, 2, COLOR_TEXT_BG);

  bm->DrawFastHLine(vp, x + METER_WIDTH - 2, y, 2, COLOR_TEXT_BG);
  bm->DrawFastVLine(vp, x + METER_WIDTH - 1, y, 2, COLOR_TEXT_BG);

  bm->DrawFastHLine(vp, x, y + METER_HEIGHT - 1, 2, COLOR_TEXT_BG);
  bm->DrawFastVLine(vp, x, y + METER_HEIGHT - 2, 2, COLOR_TEXT_BG);

  bm->DrawFastHLine(vp, x + METER_WIDTH - 2, y + METER_HEIGHT - 1, 2, COLOR_TEXT_BG);
  bm->DrawFastVLine(vp, x + METER_WIDTH - 1, y + METER_HEIGHT - 2, 2, COLOR_TEXT_BG);

  // draw the 4 pixels to round off the oval
  bm->WritePixel(x + 1, y + 1, COLOR_METER_OUTLINE);
  bm->WritePixel(x + METER_WIDTH - 2, y + 1, COLOR_METER_OUTLINE);

  bm->WritePixel(x + 1, y + METER_HEIGHT - 2, COLOR_METER_OUTLINE);
  bm->WritePixel(x + METER_WIDTH - 2, y + METER_HEIGHT - 2, COLOR_METER_OUTLINE);

  char buf[16];
  TInt length = 0;
  sprintf(buf, "%d/%d%n", value, max, &length);
  // center-align text
  x += METER_WIDTH / 2 - 4 * length;
  bm->DrawStringShadow(vp, buf, gFont8x8, x, y + 3, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);
}

static void render_item_slot(BViewPort *aViewPort, TUint16 aItemNumber, TInt aX, TInt aY, TBool aIsSelected, TBool aShowCounter = EFalse) {
  BBitmap *bm = gDisplay.renderBitmap;
  TInt i = GPlayer::mInventoryList.ItemCount(aItemNumber);
  if (aShowCounter) {
    char buf[16];
    TInt length = 0;
    sprintf(buf, "x%d%n", i, &length);
    bm->DrawString(aViewPort, buf, gFont8x8, aX + 16 - 4 * length, aY + 36, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  }

  if (i) {
    TInt16 img = items[aItemNumber];

    BBitmap *env = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
    TInt bw = gResourceManager.BitmapWidth(ENVIRONMENT_SLOT),
      bh = gResourceManager.BitmapHeight(ENVIRONMENT_SLOT),
      pitch = env->Width() / bw;

    TRect srcRect;
    srcRect.x1 = (img % pitch) * bw;
    srcRect.x2 = srcRect.x1 + bw - 1;
    srcRect.y1 = (img / pitch) * bh;
    srcRect.y2 = srcRect.y1 + bh - 1;

    bm->DrawBitmapTransparent(aViewPort, env, srcRect, aX, aY);
  }
  TUint8 color = aIsSelected ? COLOR_WHITE : COLOR_METER_OUTLINE;

  bm->DrawRect(aViewPort, aX, aY, aX + 31, aY + 31, color);
}

static void render_item_row(BViewPort *aViewPort, const char *aHeading, const TUint16 *aItemRow, TInt aX, TInt aY, TInt aSelected, TInt aEquipped, TBool aShowCounter = EFalse) {
  BBitmap *bm = gDisplay.renderBitmap;

  bm->DrawString(aViewPort, aHeading, gFont8x8, aX, aY, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  aY += 12;
  for (TInt col = 0; col < 4; col++) {
    render_item_slot(aViewPort, aItemRow[col], aX, aY, aSelected == col, aShowCounter);
    if (aEquipped - 1 == col) {
      bm->DrawRect(aViewPort, aX - 2, aY - 2, aX + 33, aY + 33, COLOR_WHITE);
      // erase middles of edges from square
      bm->DrawFastHLine(aViewPort, aX + 8, aY - 2, 16, COLOR_TEXT_BG);
      bm->DrawFastHLine(aViewPort, aX + 8, aY + 33, 16, COLOR_TEXT_BG);
      bm->DrawFastVLine(aViewPort, aX - 2, aY + 8, 16, COLOR_TEXT_BG);
      bm->DrawFastVLine(aViewPort, aX + 33, aY + 8, 16, COLOR_TEXT_BG);
    }
    aX += 38;
  }
}

GInventoryPlayfield::GInventoryPlayfield(GInventory *aInventory, BViewPort *aViewPort) : mInventory(aInventory), mViewPort(aViewPort) {
}

GInventoryPlayfield::~GInventoryPlayfield() = default;

void GInventoryPlayfield::Render() {
  char buf[256];
  BBitmap *bm = gDisplay.renderBitmap;
  bm->Clear(COLOR_TEXT_BG);

  TInt x = 4, y = 4;
  TInt currentRow = mInventory->CurrentRow();
  TInt currentColumn = mInventory->CurrentColumn();

  render_item_row(mViewPort, "Potions", mInventory->ItemLayout(0), x, y, currentRow == 0 ? currentColumn : -1, 0, ETrue);
  y += 60;
  render_item_row(mViewPort, "Spell Book", mInventory->ItemLayout(1), x, y, currentRow == 1 ? currentColumn : -1, GPlayer::mEquipped.mSpellBookElement);
  y += 48;
  render_item_row(mViewPort, "Amulet", mInventory->ItemLayout(2), x, y, currentRow == 2 ? currentColumn : -1, GPlayer::mEquipped.mAmuletElement);
  y += 48;
  render_item_row(mViewPort, "Ring", mInventory->ItemLayout(3), x, y, currentRow == 3 ? currentColumn : -1, GPlayer::mEquipped.mRingElement);

  x = 160, y = 4;
  sprintf(buf, "Level: %d", GPlayer::mLevel);
  bm->DrawString(mViewPort, buf, gFont8x8, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  y += 20;
  render_meter(mViewPort, "Health", COLOR_HEALTH, x, y, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);
  y += 20;
  render_meter(mViewPort, "Mana", COLOR_MAGIC, x, y, GPlayer::mManaPotion, GPlayer::mMaxMana);
  y += 20;
  sprintf(buf, "Attack: %d", GPlayer::mAttackStrength);
  bm->DrawString(mViewPort, buf, gFont8x8, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);

  y += 20;
  bm->DrawRect(mViewPort, x, y, x + 111, y + 95, COLOR_METER_OUTLINE);

  y = 194;
  render_meter(mViewPort, "Exp.", COLOR_EXPERIENCE, x, y, GPlayer::mExperience, GPlayer::mNextLevel);

  x = SCREEN_WIDTH - 40, y = 80;
  render_item_slot(mViewPort, ITEM_GLOVES, x, y, EFalse);
  y += 36;
  render_item_slot(mViewPort, ITEM_SWORD, x, y, EFalse);
  y += 36;
  render_item_slot(mViewPort, ITEM_BOOTS, x, y, EFalse);

  TInt16 itemNumber = mInventory->ItemNumber();
  if (itemNumber) {
    if (currentRow == 0) {
      sprintf(buf, "A to use %s", itemNames[itemNumber]);
    } else {
      sprintf(buf, "A to equip %s", itemNames[itemNumber]);
    }
    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -5);
  }
//  else {
//    sprintf(buf, "Explore to find this item.");
//    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -5);
//  }

}
