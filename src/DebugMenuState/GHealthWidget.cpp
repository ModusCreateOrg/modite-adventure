#include "Game.h"
#include "GHealthWidget.h"
#include "GPlayer.h"

GHealthWidget::GHealthWidget() : GButtonWidget("Fill Health") {
  mHeight = 20;
}

TInt GHealthWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return mHeight; //gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight;
}

void GHealthWidget::Select() {
  GPlayer::mHitPoints = GPlayer::mMaxHitPoints;
#ifdef ENABLE_AUDIO
  gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV, 2);
#endif
}
