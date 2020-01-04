#include "TOptions.h"

// bump this each time the struct is edited in IDE/debugger!
static const TInt VERSION = 1;

TOptions::TOptions() {
  Load();
}

void TOptions::SetDefaults() {
  version = VERSION;
  muted = EFalse;
  music = 0.5f;
  sfx = 0.875f;
  brightness = 0.875f;

  ResetGameProgress();
}

void TOptions::Reset(TBool aSave) {
  SetDefaults();

  if (aSave) {
    Save();
  }
}

void TOptions::ResetGameProgress() {
  gameProgress = GameProgress();
}

void TOptions::Save() {
  version = VERSION;
  BStore f("ModiteAdventure");
  f.Set("options", this, sizeof(TOptions));
}

void TOptions::Load() {
  BStore f("ModiteAdventure");
  if (!f.Get("options", this, sizeof(TOptions)) || version != VERSION) {
    Reset();
  }
}
