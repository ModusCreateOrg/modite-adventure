#include "GDoorProcess.h"
#include "GGamePlayfield.h"
#include "GPlayer.h"
#include "BSpriteSheet.h"

GDoorProcess::GDoorProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY, ENVIRONMENT_PRIORITY_DOOR) {

  mHorizontal = aHorizontal;

  if (mHorizontal) {
    // Bottom Part (The controlling factor)
    mSprite = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
      aWood ? IMG_WOOD_DOOR_H_BOTTOM : IMG_METAL_GATE_H_BOTTOM,
      STYPE_OBJECT);

    mSprite->Name(aWood ? "ENVIRONMENT HORIZONTAL WOOD DOOR BOTTOM" : "ENVIRONMENT HORIZONTAL METAL GATE BOTTOM");
    mAttribute = aWood ? ATTR_WOOD_DOOR_H : ATTR_METAL_GATE_H;

    mSprite->SetCMask(STYPE_PBULLET | STYPE_PLAYER | STYPE_ENEMY);

    mSprite->w = 32;
    mSprite->h = 64;
    mSprite->cx = -16;
    mSprite->x = aX;
    mSprite->y = aY + (mHorizontal ? 2.0 : 3.0);

    // Top Part
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_H_TOP : IMG_METAL_GATE_H_TOP, STYPE_OBJECT);

    mSprite2->Name(aWood ? "IGNORE VERTICAL WOOD DOOR 2" : "IGNORE VERTICAL METAL GATE 2");
    mSprite2->SetCMask(STYPE_PBULLET | STYPE_PLAYER | STYPE_ENEMY);

    mSprite2->w = mSprite->w;
    mSprite2->h = 32;
    mSprite2->cx = -16;
    mSprite2->cy = 32;
    mSprite2->y = mSprite->y - 32;
    mSprite2->x = mSprite->x;

  }
  else {
    // Bottom Part (The controlling factor)
    mSprite = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                aWood ? IMG_WOOD_DOOR_V_BOTTOM : IMG_METAL_GATE_V_BOTTOM,
                                STYPE_OBJECT);

    mSprite->Name(aWood ? "ENVIRONMENT HORIZONTAL WOOD DOOR BOTTOM" : "ENVIRONMENT HORIZONTAL METAL GATE BOTTOM");
    mAttribute = aWood ? ATTR_WOOD_DOOR_H : ATTR_METAL_GATE_H;

    mSprite->SetCMask(STYPE_PBULLET | STYPE_PLAYER | STYPE_ENEMY);

    mSprite->w = 16;
    mSprite->h = 64;
    mSprite->cx = 0;
    mSprite->x = aX;
    mSprite->y = aY + (mHorizontal ? 2.0 : 3.0);

    // Top Part
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_V_TOP : IMG_METAL_GATE_V_TOP, STYPE_OBJECT);

    mSprite2->Name(aWood ? "IGNORE VERTICAL WOOD DOOR TOP" : "IGNORE VERTICAL METAL GATE TOP");
    mSprite2->SetCMask(STYPE_PBULLET | STYPE_PLAYER | STYPE_ENEMY);

    mSprite2->w = mSprite->w;
    mSprite2->h = 32;
//    mSprite2->cx = mSprite->cx;
    mSprite2->cy = 32;
    mSprite2->x = mSprite->x;
    mSprite2->y = mSprite->y - 32;
  }


  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite);

  // Top Part (The candy)

  mSprite2->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite2);

}

GDoorProcess::~GDoorProcess() {
  ClearWall();
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }
}

void GDoorProcess::ClearWall() {
  mSprite->SetWall(EFalse);
  if (mSprite2) {
    mSprite2->SetWall(EFalse);
  }
}

TBool GDoorProcess::RunBefore() {
  // if door is in a group (related to switches, etc.), open when the group is "done"
  // "done" means player has successfully thrown the switches in the right order.
  TInt group = mObjectAttribute->attr.group;
  // check inventory for key
  if (group && group != OA_GROUP_ITEM && mGameState->mGamePlayfield->mGroupDone[group]) {
    // open door
    printf("door open group %d\n", group);

    ClearWall();
    mGameState->EndProgram(mIp);
    return EFalse;
  }

  return ETrue;
}

TBool GDoorProcess::RunAfter() {
  mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY);
  if (mSprite2) {
    mSprite2->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY);
  }

  // if is in a group, we don't open on collisions.
  TInt group = mObjectAttribute->item.group;
  if (group && group != OA_GROUP_ITEM) {
    return ETrue;
  }

  if (mSprite->TestAndClearCType(STYPE_PBULLET) || (mSprite2 && mSprite2->TestAndClearCType(STYPE_PBULLET))) {
    if (group == OA_GROUP_ITEM) {
      TUint16 item = mObjectAttribute->item.item;
      if (GPlayer::mInventoryList.UseItem(item)) {
        printf("door open player used item %d\n", item);
        ClearWall();
        mGameState->EndProgram(mIp);
        gSoundPlayer.TriggerSfx(SFX_DOOR_OPEN_WAV);
        return EFalse;
      }
      return ETrue;
    }
    printf("player opens door\n");
    ClearWall();
    mGameState->EndProgram(mIp);
    gSoundPlayer.TriggerSfx(SFX_DOOR_OPEN_WAV);

    return EFalse;
  }

  return ETrue;
}
