#ifndef GSTAT_SPRITE_H
#define GSTAT_SPRITE_H

#include <BSprite.h>
#include <BFont.h>
#include <BBitmap.h>


const TInt STAT_SIZE_8x8 = 0;
const TInt STAT_SIZE_16x16 = 1;

class GStatSprite : public BSprite {
public:
  GStatSprite(TInt aSize, const char *aMessage, TInt aImage = 0);
  ~GStatSprite() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;

protected:
  BFont *mFont;
  const char *mMessage;
};

#endif
