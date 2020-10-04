#include "Game.h"
#include "GItemWidget.h"
#include "GPlayer.h"
#include "Items.h"
#include "GStatProcess.h"
#include "GLevelUpPlayer.h"


GLevelUpPlayer::GLevelUpPlayer() : GButtonWidget("+ Player XP Level") {
  mHeight = 25;
}

TInt GLevelUpPlayer::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return mHeight;
}

void GLevelUpPlayer::Select() {
  GPlayer::AddExperience(GPlayer::mNextLevel);

#ifdef ENABLE_AUDIO
  gSoundPlayer.TriggerSfx(SFX_PLAYER_LEVEL_UP_WAV, 2);
#endif
}