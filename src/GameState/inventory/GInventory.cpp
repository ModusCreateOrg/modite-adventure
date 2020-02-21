#include "GInventory.h"
#include "GInventoryPlayfield.h"
#include "GPlayer.h"
#include "GResources.h"
#include "Items.h"
#include "GGameState.h"
#include <Display.h>
#include <BSpriteSheet.h>

static const char ITEM_ROW_NAMES[4][12] = {"Potions", "Spell Book", "Amulet", "Ring"};
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
}



GInventory::~GInventory() {
}

static void render_meter(BViewPort *vp, BBitmap *screen, TUint8 color, TInt x, TInt y, TInt value, TInt max) {
  TFloat pct = TFloat(value) / max;

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

  char buf[16];
  sprintf(buf, "%d/%d", value, max);
  screen->DrawStringShadow(vp, buf, gFont8x8, x + 20, y + 2, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);
}

static void render_item_slot(TUint16 aItemNumber, BViewPort *aViewPort, TInt aX, TInt aY, TBool aIsSelected, TBool aShowCounter = EFalse) {
  BBitmap *bm = gDisplay.renderBitmap;
  if (aShowCounter) {
    TInt i = GPlayer::mInventoryList.ItemCount(aItemNumber);
    char buf[16];
    sprintf(buf, "x%d", i);
    bm->DrawString(aViewPort, buf, gFont8x8, aX + 8, aY + 36, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  }
  TUint8 color = aIsSelected ? COLOR_TEXT : COLOR_METER_OUTLINE;

  if (aShowCounter || GPlayer::mInventoryList.FindItem(aItemNumber)) {
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

  bm->DrawFastHLine(aViewPort, aX, aY, 32, color);
  bm->DrawFastHLine(aViewPort, aX, aY + 31, 32, color);
  bm->DrawFastVLine(aViewPort, aX, aY + 1, 30, color);
  bm->DrawFastVLine(aViewPort, aX + 31, aY + 1, 30, color);
}

void GInventory::RenderInventory() {
  BBitmap *bm = gDisplay.renderBitmap;

  TInt x, y;
  TBool isSelected;
  for (TInt row = 0; row < 4; row++) {
    y = row ? row * 44 + 20 : 4;
    bm->DrawString(mViewPort, ITEM_ROW_NAMES[row], gFont8x8, 4, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
    for (TInt col = 0; col < 4; col++) {
      x = col * 36 + 4;
      isSelected = row == mCurrentRow && col == mCurrentColumn;
      render_item_slot(ITEM_LAYOUT[row][col], mViewPort, x, y + 8, isSelected, !row);
    }
  }

  char buf[256];
  sprintf(buf, "Level: %d", GPlayer::mLevel);
  bm->DrawString(mViewPort, buf, gFont8x8, 160, 4, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  bm->DrawString(mViewPort, "Health", gFont8x8, 160, 26, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  render_meter(mViewPort, bm, COLOR_HEALTH, 220, 24, GPlayer::mHitPoints, GPlayer::mMaxHitPoints);
  bm->DrawString(mViewPort, "Magic", gFont8x8, 160, 46, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  render_meter(mViewPort, bm, COLOR_MAGIC, 220, 44, GPlayer::mManaPotion, 100);
  sprintf(buf, "Attack: %d", GPlayer::mAttackStrength);
  bm->DrawString(mViewPort, buf, gFont8x8, 160, 64, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);

  bm->DrawString(mViewPort, "Exp.", gFont8x8, 160, 200, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
  render_meter(mViewPort, bm, COLOR_EXPERIENCE, 220, 198, GPlayer::mExperience, GPlayer::mNextLevel);

  TRect srcRect;
  BSpriteInfo *info = GPlayer::mSprite->mSpriteSheet->GetSpriteInfo(GPlayer::mSprite->mImageNumber);
  srcRect.x1 = info->x1;
  srcRect.y1 = info->y1;
  srcRect.x2 = info->x2;
  srcRect.y2 = info->y2;
  bm->DrawBitmapTransparent(mViewPort, gResourceManager.GetBitmap(PLAYER_SLOT), srcRect, 200, 120, GPlayer::mSprite->flags >> 6u);

  /*
  for (GInventoryItem *i = GPlayer::mInventoryList.First(); !GPlayer::mInventoryList.End( i); i = GPlayer::mInventoryList.Next(i)) {
    if (count >= mTop && rendered < RENDER_ITEM_MAX) {
      TRect srcRect;
      srcRect.x1 = (i->mImage % pitch) * bw;
      srcRect.x2 = srcRect.x1 + bw - 1;
      srcRect.y1 = (i->mImage / pitch) * bh;
      srcRect.y2 = srcRect.y1 + bh - 1;

      if (i->mCount > 1) {
        sprintf(buf, "%2d %-18s", i->mCount, itemNames[i->mItemNumber]); // , i->mImage);
      }
      else {
        sprintf(buf, "   %-18s", itemNames[i->mItemNumber]); // , i->mImage);
      }
      if (count == mCurrent) {
        selected = i;
        bm->FillRect(mViewPort, 0, y, SCREEN_WIDTH - 1, y + 17, COLOR_TEXT);
        bm->DrawBitmapTransparent(mViewPort, env, srcRect, 0, y - 7);
        bm->DrawString(mViewPort, buf, gFont16x16, x, y + 1, COLOR_TEXT_BG, COLOR_TEXT_TRANSPARENT, -4);
      }
      else {
        bm->DrawBitmapTransparent(mViewPort, env, srcRect, 0, y - 7);
        bm->DrawString(mViewPort, buf, gFont16x16, x, y + 1, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
      }
      rendered++;
      y += 18;
    }
    count++;
  }
*/
  GInventoryItem* selected = GPlayer::mInventoryList.FindItem(ITEM_LAYOUT[mCurrentRow][mCurrentColumn]);
  if (selected) {
    sprintf(buf, "A to use %s", itemNames[selected->mItemNumber]);
    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
    if (gControls.WasPressed(BUTTONA)) {
      switch (selected->mItemNumber) {
        case ITEM_WATER_AMULET:
          GPlayer::mEquipped.mWaterAmulet = selected;
          break;
        case ITEM_FIRE_AMULET:
          GPlayer::mEquipped.mFireAmulet = selected;
          break;
        case ITEM_EARTH_AMULET:
          GPlayer::mEquipped.mEarthAmulet = selected;
          break;
        case ITEM_ENERGY_AMULET:
          GPlayer::mEquipped.mEnergyAmulet = selected;
          break;

        case ITEM_WATER_RING:
          GPlayer::mEquipped.mWaterRing = selected;
          break;
        case ITEM_FIRE_RING:
          GPlayer::mEquipped.mFireRing = selected;
          break;
        case ITEM_EARTH_RING:
          GPlayer::mEquipped.mEarthRing = selected;
          break;
        case ITEM_ENERGY_RING:
          GPlayer::mEquipped.mEnergyRing = selected;
          break;

        case ITEM_WATER_SPELLBOOK:
        case ITEM_FIRE_SPELLBOOK:
        case ITEM_EARTH_SPELLBOOK:
        case ITEM_ENERGY_SPELLBOOK:
          GPlayer::mEquipped.mSpellBook = selected;
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
          gGame->ToggleInventory();
          break;
        case ITEM_BLUE_POTION2:
          GPlayer::mManaPotion = 100;
          selected->mCount--;
          if (selected->mCount < 1) {
            selected->Remove();
            delete selected;
          }
          gGame->ToggleInventory();
          break;
        case ITEM_RED_POTION1:
          GPlayer::mHealthPotion += 50;
          if (GPlayer::mHealthPotion > 100) {
            GPlayer::mHealthPotion = 100;
          }
          selected->mCount--;
          if (selected->mCount < 1) {
            selected->Remove();
            delete selected;
          }
          gGame->ToggleInventory();
          break;
        case ITEM_RED_POTION2:
          GPlayer::mHealthPotion = 100;
          selected->mCount--;
          if (selected->mCount < 1) {
            selected->Remove();
            delete selected;
          }
          gGame->ToggleInventory();
          break;
        default:
          break;
      }
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
/*
static void render_equipped_item(const char *aSlot, GInventoryItem *aItem, BViewPort *aViewPort, TInt x, TInt y) {
  char buf[256];
  BBitmap *bm = gDisplay.renderBitmap;

  if (!aItem) {
    sprintf(buf, "%8s:  None", aSlot);
    bm->DrawString(aViewPort, buf, gFont16x16, x, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
    return;
  }

  BBitmap *env = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
  TInt bw = gResourceManager.BitmapWidth(ENVIRONMENT_SLOT),
       bh = gResourceManager.BitmapHeight(ENVIRONMENT_SLOT),
       pitch = env->Width() / bw;

  TRect srcRect;
  srcRect.x1 = (aItem->mImage % pitch) * bw;
  srcRect.x2 = srcRect.x1 + bw - 1;
  srcRect.y1 = (aItem->mImage / pitch) * bh;
  srcRect.y2 = srcRect.y1 + bh - 1;

  bm->DrawBitmapTransparent(aViewPort, env, srcRect, x + 13 * 8, y - 7);

  sprintf(buf, "%8s:  %s", aSlot, itemNames[aItem->mItemNumber]);
  bm->DrawString(aViewPort, buf, gFont16x16, x, y + 1, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
}

void GInventory::RenderEquipped() {
  TInt y = 8,
       x = 8;

  BBitmap *bm = gDisplay.renderBitmap;

  bm->DrawString(mViewPort, "EQUIPPED", gFont16x16, 160 - 7 * 12 / 2, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
  y += 32;

//  printf("**** TODO: Render inventory screen ***\n");

  render_equipped_item("Weapon", GPlayer::mEquipped.mSword, mViewPort, x, y);
  y += 18;
  render_equipped_item("Spells", GPlayer::mEquipped.mSpellBook, mViewPort, x, y);
  y += 18;
  render_equipped_item("Amulet", GPlayer::mEquipped.mEarthAmulet, mViewPort, x, y);
  y += 18;
  render_equipped_item("Ring", GPlayer::mEquipped.mEarthRing, mViewPort, x, y);
  y += 18;
  render_equipped_item("Gloves", GPlayer::mEquipped.mGloves, mViewPort, x, y);
  y += 18;
  render_equipped_item("Boots", GPlayer::mEquipped.mBoots, mViewPort, x, y);
  y += 18;

  if (gControls.WasPressed(JOYRIGHT | JOYLEFT)) {
    mMode = MODE_INVENTORY;
  }
}
*/
void GInventory::PostRender() {
  if (gControls.WasPressed(BUTTONR)) {
    gGame->ToggleInventory();
  }

  BBitmap *bm = gDisplay.renderBitmap;
  bm->SetColor(COLOR_TEXT_BG, 0, 0, 0);
  bm->SetColor(COLOR_TEXT, 255, 255, 255);

  bm->Clear(COLOR_TEXT_BG);

  RenderInventory();
}
