#include "GInventory.h"
#include "GInventoryPlayfield.h"
#include "GPlayer.h"
#include "GResources.h"
#include "Items.h"
#include "GGameState.h"
#include <Display.h>

// How many items to render in the list view (e.g. the height of the list view in rows)
const TInt RENDER_ITEM_MAX = 8;
const TInt MODE_INVENTORY = 0;
const TInt MODE_EQUIPPED = 1;

GInventory::GInventory(BViewPort *aViewPort) : BGameEngine(aViewPort) {
  mPlayfield = new GInventoryPlayfield(this);
  mTop = 0;
  mCurrent = 0;
  mMode = MODE_INVENTORY;
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
}



GInventory::~GInventory() {
}

void GInventory::RenderInventory() {
  TInt y = 8,
       x = 24;

  BBitmap *bm = gDisplay.renderBitmap;

  bm->DrawString(mViewPort, "INVENTORY", gFont16x16, 160 - 9 * 12 / 2, y, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
  y += 32;

  BBitmap *env = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
  TInt bw = gResourceManager.BitmapWidth(ENVIRONMENT_SLOT),
       bh = gResourceManager.BitmapHeight(ENVIRONMENT_SLOT),
       pitch = env->Width() / bw;

  char buf[256];
  TInt count = 0, rendered = 0;
  GInventoryItem *selected = ENull;
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

  if (selected) {
    sprintf(buf, "A to use %s", itemNames[selected->mItemNumber]);
    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
    if (gControls.WasPressed(BUTTONA)) {
      switch (selected->mItemNumber) {
        case ITEM_WATER_AMULET:
          GPlayer::mEquipped.mWaterAmulet = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_FIRE_AMULET:
          GPlayer::mEquipped.mFireAmulet = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_EARTH_AMULET:
          GPlayer::mEquipped.mEarthAmulet = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_ENERGY_AMULET:
          GPlayer::mEquipped.mEnergyAmulet = selected;
          mMode = MODE_EQUIPPED;
          break;

        case ITEM_WATER_RING:
          GPlayer::mEquipped.mWaterRing = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_FIRE_RING:
          GPlayer::mEquipped.mFireRing = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_EARTH_RING:
          GPlayer::mEquipped.mEarthRing = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_ENERGY_RING:
          GPlayer::mEquipped.mEnergyRing = selected;
          mMode = MODE_EQUIPPED;
          break;

        case ITEM_WATER_SPELLBOOK:
        case ITEM_FIRE_SPELLBOOK:
        case ITEM_EARTH_SPELLBOOK:
        case ITEM_ENERGY_SPELLBOOK:
          GPlayer::mEquipped.mSpellBook = selected;
          mMode = MODE_EQUIPPED;
          break;

        case ITEM_BOOTS:
          GPlayer::mEquipped.mBoots = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_GLOVES:
          GPlayer::mEquipped.mGloves = selected;
          mMode = MODE_EQUIPPED;
          break;
        case ITEM_SWORD:
          GPlayer::mEquipped.mSword = selected;
          GPlayer::mAttackStrength += 20;
          mMode = MODE_EQUIPPED;
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
    if (mCurrent) {
      mCurrent--;
      if (mCurrent < mTop) {
        mTop = mCurrent;
      }
    }
  }

  if (gControls.WasPressed(JOYUP)) {
    if (mCurrent) {
      mCurrent--;
      if (mTop < mCurrent) {
        mTop = mCurrent;
      }
    }
  }

  if (gControls.WasPressed(JOYDOWN)) {
    if (mCurrent < count - 1) {
      mCurrent++;
      if (mCurrent > rendered) {
        mTop++;
      }
    }
  }
  if (gControls.WasPressed(JOYRIGHT | JOYLEFT)) {
    mMode = MODE_EQUIPPED;
  }
}

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

void GInventory::PostRender() {
  if (gControls.WasPressed(BUTTONR)) {
    gGame->ToggleInventory();
  }

  BBitmap *bm = gDisplay.renderBitmap;
  bm->SetColor(COLOR_TEXT_BG, 0, 0, 0);
  bm->SetColor(COLOR_TEXT, 255, 255, 255);

  bm->Clear(COLOR_TEXT_BG);
  bm->DrawString(mViewPort, "<", gFont16x16, 0, 0, COLOR_TEXT);
  bm->DrawString(mViewPort, ">", gFont16x16, 319 - 12, 0, COLOR_TEXT);

  switch (mMode) {
    case MODE_INVENTORY:
      RenderInventory();
      break;
    case MODE_EQUIPPED:
    default:
      RenderEquipped();
      break;
  }
}
