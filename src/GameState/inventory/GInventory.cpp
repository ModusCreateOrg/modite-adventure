#include "GInventory.h"
#include "GInventoryPlayfield.h"
#include "GPlayer.h"
#include "Items.h"

GInventory::GInventory(BViewPort *aViewPort) : BGameEngine(aViewPort) {
  mPlayfield = new GInventoryPlayfield(this);
  mTop = 0;
  mCurrent = 0;
}

GInventory::~GInventory() {
}

void GInventory::PostRender() {
  TInt y = 8;
  TInt x = 24;
  const TInt RENDER_ITEM_MAX = 8;

  BBitmap *bm = gDisplay.renderBitmap;
  bm->SetColor(COLOR_TEXT_BG, 0, 0, 0);
  bm->SetColor(COLOR_TEXT, 255, 255, 255);

  bm->Clear(COLOR_TEXT_BG);
  bm->DrawString(mViewPort, "INVENTORY", gFont16x16, 160-9*12/2, y, COLOR_TEXT, 0, -4);
  y += 32;

  BBitmap *env = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
  TInt bw = gResourceManager.BitmapWidth(ENVIRONMENT_SLOT),
       bh = gResourceManager.BitmapHeight(ENVIRONMENT_SLOT),
       pitch = env->Width() / bw;

  char buf[256];
  TInt count = 0, rendered = 0;
  GInventoryItem *selected = ENull;
  for (GInventoryItem *i = GPlayer::mInventoryList.First(); !GPlayer::mInventoryList.End(i); i = GPlayer::mInventoryList.Next(i)) {
    if (count >= mTop && rendered < RENDER_ITEM_MAX) {
      //    if (mImageNumber) {
      //      y -= 8;
      //      BSprite::Render(aViewPort);
      //      screenX += gResourceManager.BitmapWidth(mBitmapSlot);
      //      y += 8;
      //    }

      TRect srcRect;
      srcRect.x1 = (i->mImage % pitch) * bw;
      srcRect.x2 = srcRect.x1 + bw - 1;
      srcRect.y1 = (i->mImage / pitch) * bh;
      srcRect.y2 = srcRect.y1 + bh - 1;
      sprintf(buf, "%2d %-18s", i->mCount, itemNames[i->mItemNumber]); // , i->mImage);
      if (count == mCurrent) {
        selected = i;
        bm->FillRect(mViewPort, 0, y, SCREEN_WIDTH - 1, y + 17, COLOR_TEXT);
        bm->DrawBitmapTransparent(mViewPort, env, srcRect, 0, y - 7);
        bm->DrawString(mViewPort, buf, gFont16x16, x, y+1, COLOR_TEXT_BG, -1, -4);
      }
      else {
        bm->DrawBitmapTransparent(mViewPort, env, srcRect, 0, y - 7);
        bm->DrawString(mViewPort, buf, gFont16x16, x, y+1, COLOR_TEXT, -1, -4);
      }
      rendered++;
      y += 18;
    }
    count++;
  }

  if (selected) {
    sprintf(buf, "A to use %s", itemNames[selected->mItemNumber]);
    bm->DrawString(mViewPort, buf, gFont16x16, 4, 220, COLOR_TEXT, 0, -4);
  }

  if (gControls.WasPressed(BUTTONR)) {
    gGame->ToggleInventory();
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
}
