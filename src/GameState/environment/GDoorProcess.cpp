#include "GDoorProcess.h"
#include "GGamePlayfield.h"
#include "GPlayer.h"
#include "BSpriteSheet.h"

//OAL value model for closing doors
//16 * (ATTR_BLUE_SPELLBOOK) + 15
GDoorProcess::GDoorProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWood, TBool aHorizontal)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY, ENVIRONMENT_PRIORITY_DOOR) {

  mHorizontal = aHorizontal;

  if (mHorizontal) {
    // Bottom Part (The controlling factor)
    mSprite = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
      aWood ? IMG_WOOD_DOOR_H_BOTTOM : IMG_METAL_GATE_H_BOTTOM,
                                STYPE_OBJECT);

    mSprite->Name(aWood ? "WOOD DOOR H BOTTOM" : "METAL GATE H BOTTOM");
    mAttribute = aWood ? ATTR_WOOD_DOOR_H : ATTR_METAL_GATE_H;

    mSprite->SetCMask(STYPE_PBULLET);

    mSprite->w = 32;
    mSprite->h = 16;
    mSprite->cx = -16;
    mSprite->x = aX;
    mSprite->y = aY;

    mSprite->SetMapAttribute(ATTR_HORIZONTAL_DOOR_WALL << 3u);

    // Top Part
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_H_TOP : IMG_METAL_GATE_H_TOP, STYPE_DEFAULT);

    mSprite2->Name(aWood ? "WOOD DOOR H TOP" : "METAL GATE H TOP");
  }
  else {
    // Bottom Part (The controlling factor)
    mSprite = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                aWood ? IMG_WOOD_DOOR_V_BOTTOM : IMG_METAL_GATE_V_BOTTOM,
                                STYPE_OBJECT);

    mSprite->Name(aWood ? "WOOD DOOR V BOTTOM" : "METAL GATE V BOTTOM");
    mAttribute = aWood ? ATTR_WOOD_DOOR_H : ATTR_METAL_GATE_H;

    mSprite->SetCMask(STYPE_PBULLET);

    mSprite->w = 16;
    mSprite->h = 32;
    mSprite->cx = 0;
    mSprite->x = aX;
    mSprite->y = aY;

    mSprite->SetMapAttribute(ATTR_VERTICAL_DOOR_WALL << 3u);

    // Top Part
    mSprite2 = new GAnchorSprite(mGameState, DOOR_PRIORITY, ENVIRONMENT_SLOT,
                                 aWood ? IMG_WOOD_DOOR_V_TOP : IMG_METAL_GATE_V_TOP, STYPE_DEFAULT);

    mSprite2->Name(aWood ? "WOOD DOOR V TOP" : "METAL GATE V TOP");
  }
  mSprite2->x = mSprite->x;
  mSprite2->y = mSprite->y;
  mSprite2->mDy = -32;


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
  mSprite->ResetMapAttribute();
}

TBool GDoorProcess::RunBefore() {
  // if door is in a group (related to switches, etc.), open when the group is "done"
  // "done" means player has successfully thrown the switches in the right order.
  TInt group = mObjectAttribute->attr.group;
  // check inventory for key
  if (group && group != OA_GROUP_ITEM && mGameState->mGamePlayfield->mGroupDone[group]) {
    // open door
//    printf("door open group %d\n", group);
    gSoundPlayer.TriggerSfx(SFX_DOOR_OPEN_GROUP_WAV, 5);
    ClearWall();
    mGameState->EndProgram(mIp);
    return EFalse;
  }

  return ETrue;
}

TBool GDoorProcess::RunAfter() {
  const TBool collided = mSprite->TestAndClearCType(STYPE_PBULLET);

  // if is in a group, we don't open on collisions.
  TInt group = mObjectAttribute->item.group;
  if (group && group != OA_GROUP_ITEM) {
    return ETrue;
  }

  if (collided) {
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
