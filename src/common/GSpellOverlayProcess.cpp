#include "GSpellOverlayProcess.h"
#include "GGameState.h"
#include "GResources.h"
#include "GPlayer.h"

static TInt16 SPELL_SPEED = 4;

static ANIMSCRIPT spellOverlayAnimation[] = {
  ASTEP(SPELL_SPEED, IMG_SPELL),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 4),
  ASTEP(SPELL_SPEED, IMG_SPELL + 5),
  ASTEP(SPELL_SPEED, IMG_SPELL + 6),
  ASTEP(SPELL_SPEED, IMG_SPELL + 7),
  ASTEP(SPELL_SPEED, IMG_SPELL + 8),
  AEND
};

GSpellOverlayProcess::GSpellOverlayProcess(GGameState *aGameState, GProcess *aProcess, TFloat aX, TFloat aY,
                                           TInt16 aSpawnDelay, TFloat aVX = 0, TFloat aVY = 0)
    : GProcess(ATTR_GONE) {
  mTimer = 0;
  mSprite = new GAnchorSprite(aGameState, ENEMY_SPELL_PRIORITY, GPlayer::GetSpellSlot());
  mParent = aProcess;
  mSprite->x = aX;

  mSprite->vx = aVX;
  mSprite->vy = aVY;

  switch (GPlayer::mEquipped.mSpellBookElement) {
    case ELEMENT_WATER:
      mSprite->y = aY + 16;
      break;
    case ELEMENT_FIRE:
    case ELEMENT_EARTH:
    case ELEMENT_ENERGY:
      mSprite->y = aY + 1;
      break;
    default:
      Panic("Invalid spell");
  }

  mSpawnDelay = aSpawnDelay;
  mGameState = aGameState;
}

GSpellOverlayProcess::~GSpellOverlayProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GSpellOverlayProcess::RunBefore() {
  if (mTimer == mSpawnDelay) {
    mGameState->AddSprite(mSprite);
    mSprite->StartAnimation(spellOverlayAnimation);
  }
  mTimer++;

  return ETrue;
}

TBool GSpellOverlayProcess::RunAfter() {
  if (mSprite->AnimDone()) {
    gEventEmitter.FireEvent(this, EVENT_SPELL_PROCESS_EXIT, ENull);
    mParent->OverlayAnimationComplete();
    return EFalse;
  }

  return ETrue;
}
