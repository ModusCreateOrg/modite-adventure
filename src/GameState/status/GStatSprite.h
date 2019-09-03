#ifndef GSTAT_SPRITE_H
#define GSTAT_SPRITE_H

#include <BSprite.h>
#include <BFont.h>
#include <BBitmap.h>

enum STAT_SIZE {
  STAT_SIZE_8x8,
  STAT_SIZE_16x16,
};

class GStatSprite : public BSprite {
public:
  GStatSprite(STAT_SIZE aSize, const char *aMessage);
  ~GStatSprite() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;

protected:
  BFont *mFont;
  const char *mMessage;
};

#endif
