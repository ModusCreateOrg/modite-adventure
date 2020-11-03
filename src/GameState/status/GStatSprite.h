#ifndef GSTAT_SPRITE_H
#define GSTAT_SPRITE_H

#include <BSprite.h>
#include <BFont.h>
#include <BBitmap.h>
#include "GAnchorSprite.h"

enum STAT_TYPE {
  STAT_PLAYER_HIT,
  STAT_ENEMY_HIT,
  STAT_HEAL,
  STAT_MANA,
  STAT_EXPERIENCE,
  STAT_INFO,
};

class GStatSprite : public BSprite {
public:
  GStatSprite(STAT_TYPE aType, const char *aMessage, TInt aImage = 0);
  ~GStatSprite() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;

protected:
  char *mMessage;
  STAT_TYPE mMessageType;
};

#endif
