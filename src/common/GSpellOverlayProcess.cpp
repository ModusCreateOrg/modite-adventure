#include "GSpellOverlayProcess.h"
#include "GGameState.h"
#include "GResources.h"
#include "GPlayer.h"

static TInt16 SPELL_SPEED = 5;

static ANIMSCRIPT spellOverlayAnimation[] = {
  ASTEP(SPELL_SPEED, IMG_SPELL + 0),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 4),
  ASTEP(SPELL_SPEED, IMG_SPELL + 5),
  AEND,
};

GSpellOverlayProcess::GSpellOverlayProcess(GGameState *aGameState, GProcess *aProcess, TFloat aX, TFloat aY)
    : GProcess(ATTR_GONE) {
  mSprite = new GAnchorSprite(aGameState, ENEMY_SPELL_PRIORITY, GPlayer::GetSpellSlot());
  mParent = aProcess;
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->StartAnimation(spellOverlayAnimation);
  aGameState->AddSprite(mSprite);
}

GSpellOverlayProcess::~GSpellOverlayProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GSpellOverlayProcess::RunBefore() {
  return ETrue;
}

TBool GSpellOverlayProcess::RunAfter() {
  if (mSprite->AnimDone()) {
    gEventEmitter.FireEvent(this, EVENT_SPELL_PROCESS_EXIT, ENull );
    mParent->OverlayAnimationComplete();
    return EFalse;
  }
  return ETrue;
}
