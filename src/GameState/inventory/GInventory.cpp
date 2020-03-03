#include "GInventory.h"
#include "GInventoryPlayfield.h"
#include "GPlayer.h"
#include "GResources.h"
#include "Items.h"
#include "GGameState.h"
#include <Display.h>
#include <BSpriteSheet.h>

static const TUint16 ITEM_LAYOUT[4][4] = {
  { ITEM_RED_POTION1, ITEM_RED_POTION2, ITEM_BLUE_POTION1, ITEM_BLUE_POTION2 },
  { ITEM_WATER_SPELLBOOK, ITEM_FIRE_SPELLBOOK, ITEM_EARTH_SPELLBOOK, ITEM_ENERGY_SPELLBOOK },
  { ITEM_WATER_AMULET, ITEM_FIRE_AMULET, ITEM_EARTH_AMULET, ITEM_ENERGY_AMULET },
  { ITEM_WATER_RING, ITEM_FIRE_RING, ITEM_EARTH_RING, ITEM_ENERGY_RING }
};
static const TInt METER_WIDTH = 100;
static const TInt METER_HEIGHT = 12;

GInventory::GInventory(BViewPort *aViewPort) : BGameEngine(aViewPort) {
  mPlayfield = new GInventoryPlayfield(this);
  mCurrentColumn = mCurrentRow = 0;
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_METER_OUTLINE, 64, 64, 64);
}



GInventory::~GInventory() {
}

static void render_meter(BViewPort *vp, BBitmap *bm, const char *name, TUint8 color, TInt x, TInt y, TInt value, TInt max) {
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

void GInventory::RenderInventory() {
  BBitmap *bm = gDisplay.renderBitmap;
  char buf[256];

  TInt x = 4, y = 4;
  render_item_row(mViewPort, "Potions", ITEM_LAYOUT[0], x, y, mCurrentRow == 0 ? mCurrentColumn : -1, 0, ETrue);
  y += 60;
  render_item_row(mViewPort, "Spell Book", ITEM_LAYOUT[1], x, y, mCurrentRow == 1 ? mCurrentColumn : -1, GPlayer::mEquipped.mSpellBookElement);
  y += 48;
  render_item_row(mViewPort, "Amulet", ITEM_LAYOUT[2], x, y, mCurrentRow == 2 ? mCurrentColumn : -1, GPlayer::mEquipped.mAmuletElement);
  y += 48;
  render_item_row(mViewPort, "Ring", ITEM_LAYOUT[3], x, y, mCurrentRow == 3 ? mCurrentColumn : -1, GPlayer::mEquipped.mRingElement);

  x = 160, y = 4;
  sprintf(buf, "Level: %d", GPlayer::mLevel);
  bm->DrawString(mViewPort, buf, gFont8x8, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  y += 20;
  render_meter(mViewPort, bm, "Health", COLOR_HEALTH, x, y, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);
  y += 20;
  render_meter(mViewPort, bm, "Magic", COLOR_MAGIC, x, y, GPlayer::mManaPotion, 100);
  y += 20;
  sprintf(buf, "Attack: %d", GPlayer::mAttackStrength);
  bm->DrawString(mViewPort, buf, gFont8x8, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);

  y += 20;
  bm->DrawRect(mViewPort, x, y, x + 111, y + 95, COLOR_METER_OUTLINE);

  TInt offsetX, offsetY;
  TRect srcRect;
  BSpriteInfo *info = GPlayer::mSprite->mSpriteSheet->GetSpriteInfo(GPlayer::mSprite->mImageNumber);
  BBitmap *playerBitmap = gResourceManager.GetBitmap(PLAYER_SLOT);
  srcRect.x1 = info->x1;
  srcRect.y1 = info->y1;
  srcRect.x2 = info->x2;
  srcRect.y2 = info->y2;
  offsetX = info->dx + TInt(GPlayer::mSprite->mDx) + 24;
  offsetY = info->dy + TInt(GPlayer::mSprite->mDy) + 4;
  bm->DrawBitmapTransparent(mViewPort, playerBitmap, srcRect, x + offsetX, y + offsetY, GPlayer::mSprite->flags >> 6u);

  y = 194;
  render_meter(mViewPort, bm, "Exp.", COLOR_EXPERIENCE, x, y, GPlayer::mExperience, GPlayer::mNextLevel);

  x = SCREEN_WIDTH - 40, y = 80;
  render_item_slot(mViewPort, ITEM_GLOVES, x, y, EFalse);
  y += 36;
  render_item_slot(mViewPort, ITEM_SWORD, x, y, EFalse);
  y += 36;
  render_item_slot(mViewPort, ITEM_BOOTS, x, y, EFalse);

  GInventoryItem* selected = GPlayer::mInventoryList.FindItem(ITEM_LAYOUT[mCurrentRow][mCurrentColumn]);
  TInt16 itemNumber = 0;
  if (selected) {
    itemNumber = selected->mItemNumber;
    if (((itemNumber == ITEM_RED_POTION1 || itemNumber == ITEM_RED_POTION2) &&
         GPlayer::mHitPoints == GPlayer::mMaxHitPoints) ||
        ((itemNumber == ITEM_BLUE_POTION1 || itemNumber == ITEM_BLUE_POTION2) && GPlayer::mManaPotion == 100)) {
      itemNumber = 0;
    }
  }
  if (itemNumber) {
    if (mCurrentRow == 0) {
      sprintf(buf, "A to use %s", itemNames[itemNumber]);
    } else {
      sprintf(buf, "A to equip %s", itemNames[itemNumber]);
    }
    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -5);
  }
  if (gControls.WasPressed(BUTTONA) && itemNumber) {
    switch (itemNumber) {
      case ITEM_WATER_AMULET:
        GPlayer::mEquipped.mAmuletElement = ELEMENT_WATER;
        break;
      case ITEM_FIRE_AMULET:
        GPlayer::mEquipped.mAmuletElement = ELEMENT_FIRE;
        break;
      case ITEM_EARTH_AMULET:
        GPlayer::mEquipped.mAmuletElement = ELEMENT_EARTH;
        break;
      case ITEM_ENERGY_AMULET:
        GPlayer::mEquipped.mAmuletElement = ELEMENT_ENERGY;
        break;

      case ITEM_WATER_RING:
        GPlayer::mEquipped.mRingElement = ELEMENT_WATER;
        break;
      case ITEM_FIRE_RING:
        GPlayer::mEquipped.mRingElement = ELEMENT_FIRE;
        break;
      case ITEM_EARTH_RING:
        GPlayer::mEquipped.mRingElement = ELEMENT_EARTH;
        break;
      case ITEM_ENERGY_RING:
        GPlayer::mEquipped.mRingElement = ELEMENT_ENERGY;
        break;

      case ITEM_WATER_SPELLBOOK:
        GPlayer::mEquipped.mSpellBookElement = ELEMENT_WATER;
        break;
      case ITEM_FIRE_SPELLBOOK:
        GPlayer::mEquipped.mSpellBookElement = ELEMENT_FIRE;
        break;
      case ITEM_EARTH_SPELLBOOK:
        GPlayer::mEquipped.mSpellBookElement = ELEMENT_EARTH;
        break;
      case ITEM_ENERGY_SPELLBOOK:
        GPlayer::mEquipped.mSpellBookElement = ELEMENT_ENERGY;
        break;
      case ITEM_BLUE_POTION1:
        GPlayer::mManaPotion += 50;
        if (GPlayer::mManaPotion > 100) {
          GPlayer::mManaPotion = 100;
        }
        selected->mCount--;
        if (selected->mCount < 1) {
          selected->Remove();
          delete selected;
        }
        break;
      case ITEM_BLUE_POTION2:
        GPlayer::mManaPotion = 100;
        selected->mCount--;
        if (selected->mCount < 1) {
          selected->Remove();
          delete selected;
        }
        break;
      case ITEM_RED_POTION1:
        GPlayer::AddHitPoints(50, EFalse);
        selected->mCount--;
        if (selected->mCount < 1) {
          selected->Remove();
          delete selected;
        }
        break;
      case ITEM_RED_POTION2:
        GPlayer::AddHitPoints(100, EFalse);
        selected->mCount--;
        if (selected->mCount < 1) {
          selected->Remove();
          delete selected;
        }
        break;
      default:
        break;
    }
  }

  if (gControls.WasPressed(JOYUP)) {
    mCurrentRow--;
    if (mCurrentRow < 0) {
      mCurrentRow = 3;
    }
  }
  if (gControls.WasPressed(JOYDOWN)) {
    mCurrentRow++;
    if (mCurrentRow > 3) {
      mCurrentRow = 0;
    }
  }

  if (gControls.WasPressed(JOYLEFT)) {
    mCurrentColumn--;
    if (mCurrentColumn < 0) {
      mCurrentColumn = 3;
    }
  }
  if (gControls.WasPressed(JOYRIGHT)) {
    mCurrentColumn++;
    if (mCurrentColumn > 3) {
      mCurrentColumn = 0;
    }
  }
}

void GInventory::PostRender() {
  if (gControls.WasPressed(BUTTONR)) {
    if (GPlayer::mHitPointsHealed > 0) {
      GPlayer::AddHitPoints(GPlayer::mHitPointsHealed);
    }
    gGame->ToggleInventory();
  }

  BBitmap *bm = gDisplay.renderBitmap;
  bm->Clear(COLOR_TEXT_BG);

  RenderInventory();
}
