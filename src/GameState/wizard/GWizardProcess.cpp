#include "GWizardProcess.h"
#include "GEnemyProcess.h"

#define DEBUGME
//#undef DEBUGME

const TInt16 WALK_SPEED = 8;
const TInt16 WALK_VELOCITY = 1;

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(1, IMG_WIZARD_IDLE),
  AEND,
};

static ANIMSCRIPT walkDownAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 1),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 1),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 1),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 1),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 1),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 2),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 1),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 1),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 1),
  ALOOP,
};

GWizardProcess::GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet)
    : GProcess(aAttribute) {
#ifdef DEBUGME
  printf("BOSS_SLOT: %d, aSlot: %d, aType: %d, aAttribute: %x, aSpriteSheet = %d",
    BOSS_SLOT, aSlot, aType, aAttribute, aSpriteSheet);
#endif
  mGameState = aGameState;
  mSlot = aSlot;
  mIp = aIp;
  mType = aType;
  mAttribute = aAttribute;
  //
  mSaveToStream = ETrue;
  //
  mSprite = new GAnchorSprite(mGameState, 0, aSlot);
  mSprite->Name("WIZARD");
  //  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->type = STYPE_ENEMY;
  //  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->cx = 10;
  mSprite->cy = 0;
  mSprite->w = 22;
  mSprite->h = 24;
  SetState(IDLE_STATE, DIRECTION_DOWN);
  mGameState->AddSprite(mSprite);
}

GWizardProcess::~GWizardProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GWizardProcess::SetState(TInt aState, DIRECTION aDirection) {
  mState = aState;
  mDirection = aDirection;
  // set animation
  mStateTimer = 5 * 60;
  switch (mState) {
    case IDLE_STATE:
      mSprite->vx = mSprite->vy = 0;
      mSprite->StartAnimation(idleAnimation);
      break;
    case WALK_STATE:
      switch (mDirection) {
        case DIRECTION_UP:
//          mSprite->vy = -WALK_VELOCITY;
          mSprite->StartAnimation(walkUpAnimation);
          break;
        case DIRECTION_DOWN:
//          mSprite->vy = WALK_VELOCITY;
          mSprite->StartAnimation(walkDownAnimation);
          break;
        case DIRECTION_LEFT:
//          mSprite->vx = -WALK_VELOCITY;
          mSprite->StartAnimation(walkLeftAnimation);
          break;
        case DIRECTION_RIGHT:
//          mSprite->vx = WALK_VELOCITY;
          mSprite->StartAnimation(walkRightAnimation);
          break;
      }
      break;
    default:
      Panic("GWizardProcess: invalid SetState(%d)\n", mState);
  }
}

TBool GWizardProcess::RunBefore() {
  if (--mStateTimer < 1) {
    mStateTimer = 5 * 60;
    switch (mState) {
      case IDLE_STATE:
        SetState(WALK_STATE, DIRECTION_UP);
        break;
      case WALK_STATE:
        switch (mDirection) {
          case DIRECTION_UP:
            SetState(WALK_STATE, DIRECTION_RIGHT);
            break;
          case DIRECTION_DOWN:
            SetState(WALK_STATE, DIRECTION_LEFT);
            break;
          default:
          case DIRECTION_LEFT:
            SetState(IDLE_STATE, DIRECTION_DOWN);
            break;
          case DIRECTION_RIGHT:
            SetState(WALK_STATE, DIRECTION_DOWN);
            break;
        }
    }
  }
  return ETrue;
}

TBool GWizardProcess::RunAfter() {
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET);
  return ETrue;
}

void GWizardProcess::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mIp, sizeof(mIp));
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
  aStream.Write(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Write(&mStateTimer, sizeof(mStateTimer));
  mSprite->WriteToStream(aStream);
}

void GWizardProcess::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mIp, sizeof(mIp));
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
  aStream.Read(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Read(&mStateTimer, sizeof(mStateTimer));
  mSprite->ReadFromStream(aStream);
}
