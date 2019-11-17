#include "Game.h"
#include "GHealthWidget.h"
#include "GPlayer.h";

static const TRange health_options = {
  0, DEFAULT_PLAYER_HITPOINTS, DEFAULT_PLAYER_HITPOINTS / 8
};

GHealthWidget::GHealthWidget() : GSoundSliderWidget("HEALTH", &health_options, COLOR_TEXT, COLOR_TEXT_BG) {
  mHeight = 25;
}

GHealthWidget::~GHealthWidget() {}

TInt GHealthWidget::RenderTitle(TInt aX, TInt aY, TBool aActive) {
  aY += mHeight;
  return GSoundSliderWidget::RenderTitle(aX, aY, aActive);
}

TInt GHealthWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  mSelectedValue = GPlayer::mHitPoints;
  GSoundSliderWidget::Render(aX, aY);
  return 4;
}

void GHealthWidget::Select(TInt aVal) {
  GPlayer::mHitPoints = aVal;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxOptionSelect();
#endif
}
