#include "GInventoryProcess.h"
#include "GInventory.h"
#include "Items.h"
#include "GPlayer.h"
#include "GResources.h"

static const TUint8 DEBOUNCE = FRAMES_PER_SECOND / 4;
const TInt16 WALKSPEED = 4 * FACTOR;
const TInt16 QUAFF_SPEED = 3 * FACTOR;
const TUint8 PLAYER_Y = 152;

ANIMSCRIPT walkDownAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 0),
  ADELTA(0, -2),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 1),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 2),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 3),
  AEND,
  ALOOP,
};



ANIMSCRIPT quaffAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(0, 2),
  ASTEP(QUAFF_SPEED, IMG_WALK_DOWN + 1),
  ADELTA(1, 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 0),
  ADELTA(0, 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 47),
  ADELTA(-1, 0),
  ASTEP(QUAFF_SPEED * 5, IMG_IDLE + 3),
  ADELTA(0, 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 47),
  ADELTA(1, 0),
  ASTEP(QUAFF_SPEED, IMG_IDLE + 0),
  ADELTA(0, 2),
  ASTEP(QUAFF_SPEED, IMG_WALK_DOWN + 1),
  AEND,
};

GInventoryProcess::GInventoryProcess(GInventory *aGameState) {
  mGameState = aGameState;
  mItemNumber = 0;
  mCurrentColumn = mCurrentRow = 0;
  mDebounce = DEBOUNCE;

  mSprite = new GPlayerSprite((GGameState *)mGameState);
  mSprite->x = 183;
  mSprite->y = PLAYER_Y;
  mGameState->AddSprite(mSprite);
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW | SFLAG_RENDER_DEBUG);
  mSprite->StartAnimation(walkDownAnimation);
}

GInventoryProcess::~GInventoryProcess() {
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

TBool GInventoryProcess::RunBefore() {
  if (mSprite->AnimDone()) {
    mSprite->y = PLAYER_Y;
    mSprite->StartAnimation(walkDownAnimation);
  }

  if (--mDebounce < 0) {
    mDebounce = 0;
  }

  mSelectedItem = GPlayer::mInventoryList.FindItem(mGameState->ItemLayout(mCurrentRow, mCurrentColumn));
  mItemNumber = 0;
  if (mSelectedItem) {
    mItemNumber = mSelectedItem->mItemNumber;
    if (((mItemNumber == ITEM_RED_POTION1 || mItemNumber == ITEM_RED_POTION2) &&
          GPlayer::mHitPoints == GPlayer::mMaxHitPoints) ||
        ((mItemNumber == ITEM_BLUE_POTION1 || mItemNumber == ITEM_BLUE_POTION2) &&
          GPlayer::mManaPotion == GPlayer::mMaxMana)) {
      mItemNumber = 0;
    }
  }

  return ETrue;
}

TBool GInventoryProcess::RunAfter() {
  if (gControls.WasPressed(BUTTONA) && mDebounce == 0 && mItemNumber) {
    mDebounce = DEBOUNCE;
    mSprite->y = PLAYER_Y - 4;
    mSprite->mDx = mSprite->mDy = 0;
    mSprite->StartAnimation(quaffAnimation);
    gSoundPlayer.TriggerSfx(mItemNumber >= ITEM_RED_POTION1 ? SFX_ITEM_HEART_WAV : SFX_ITEM_PICKUP_GENERIC_WAV);

    switch (mItemNumber) {
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
        GPlayer::AddMana(25, mSprite->Center());
        mSelectedItem->mCount--;
        if (mSelectedItem->mCount < 1) {
          mSelectedItem->Remove();
          delete mSelectedItem;
        }
        break;
      case ITEM_BLUE_POTION2:
        GPlayer::AddMana(50, mSprite->Center());
        mSelectedItem->mCount--;
        if (mSelectedItem->mCount < 1) {
          mSelectedItem->Remove();
          delete mSelectedItem;
        }
        break;
      case ITEM_RED_POTION1:
        GPlayer::AddHitPoints(50, mSprite->Center());
        mSelectedItem->mCount--;
        if (mSelectedItem->mCount < 1) {
          mSelectedItem->Remove();
          delete mSelectedItem;
        }
        break;
      case ITEM_RED_POTION2:
        GPlayer::AddHitPoints(100, mSprite->Center());
        mSelectedItem->mCount--;
        if (mSelectedItem->mCount < 1) {
          mSelectedItem->Remove();
          delete mSelectedItem;
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
  return ETrue;
}
