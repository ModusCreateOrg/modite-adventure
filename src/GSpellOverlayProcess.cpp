#include "GSpellOverlayProcess.h"
#include "GGameState.h"
#include "GResources.h"

static TInt16 SPELL_SPEED = 2;

static ANIMSCRIPT spellOverlayAnimation[] = {
  ABITMAP(PLAYER_SPELL_SLOT),
  ASTEP(SPELL_SPEED, IMG_SPELL + 0),
  ASTEP(SPELL_SPEED, IMG_SPELL + 1),
  ASTEP(SPELL_SPEED, IMG_SPELL + 2),
  ASTEP(SPELL_SPEED, IMG_SPELL + 3),
  ASTEP(SPELL_SPEED, IMG_SPELL + 4),
  ASTEP(SPELL_SPEED, IMG_SPELL + 5),
  ASTEP(SPELL_SPEED, IMG_SPELL + 6),
  ASTEP(SPELL_SPEED, IMG_SPELL + 7),
  ASTEP(SPELL_SPEED, IMG_SPELL + 8),
  AEND,
};

GSpellOverlayProcess::GSpellOverlayProcess(GGameState *aGameState, TFloat aX, TFloat aY){
  mSprite = new GAnchorSprite(aGameState, ENEMY_SPELL_PRIORITY, PLAYER_SPELL_SLOT);
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
    return EFalse;
  }
  return ETrue;
}
