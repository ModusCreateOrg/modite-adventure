#include "GSplashPlayfield.h"



void GSplashPlayfield::InitParticles() {
  TFloat centerX = 0,
  centerY = 0;

  TInt transparentColor = mBackground->TransparentColor();

  TInt  width  = mBackground->Width(),
  height = mBackground->Height();

  centerX = (SCREEN_WIDTH * .5)  - (width * .5);
  centerY = (SCREEN_HEIGHT * .5) - (height * .5);

  printf("Height %i Width %i\n", height, width);

  mNumParticles = width * height;

  mParticles = (Particle *)malloc(mNumParticles * sizeof(Particle));

  TRGB *source = mBackground->GetPalette();
  for (TInt color = 0; color < COLOR_SHADOW; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    gDisplay.SetColor(color, 90,90,90);
  }


  for (TInt row = 0;  row < height - 1;  row++) {
    for (TInt col = 0; col < width - 1 ; col++) {
      TInt index = (row * width) + col;

      TRGB color = mBackground->ReadColor(col, row);
      TInt colorIndex = mBackground->FindColor(color);

      Particle particle;

      particle.index = index;
      particle.x = col + centerX;
      particle.y = row + centerY;
      particle.screen_x = particle.x;
      particle.screen_y = 0;

      particle.color = colorIndex == transparentColor ? 254 : colorIndex;

      mParticles[index] = particle;
    }

    gDisplay.SetColor(253, 255,255,255);
    gDisplay.SetColor(254, 0,0,0);
  }

}

GSplashPlayfield::GSplashPlayfield() {
  gResourceManager.LoadBitmap(SPLASH_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBackground = gResourceManager.GetBitmap(BKG_SLOT);
  gDisplay.SetPalette(mBackground);
  mNumParticlesMoved = 0;
  mFrame = 0;
  InitParticles();

//  Starfield::randomize();

#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(MAIN_MENU_XM);
#endif
}

GSplashPlayfield::~GSplashPlayfield() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  free(mParticles);
}

void GSplashPlayfield::Render() {
  TRect rect;

  rect.Height(mBackground->Height());
  rect.Width(mBackground->Width());

  gDisplay.renderBitmap->Clear(254);

  for (int i = 0; i < mNumParticles - 1; i++) {
    // Don't draw
//    if (mParticles[i].color == 254) {
//      continue;
//    }

    // Draw the star at its new coordinate.
    gDisplay.renderBitmap->SafeWritePixel(
      mParticles[i].screen_x,
      mParticles[i].screen_y,
      mParticles[i].color
    );
  }

  mFrame++;
//  gDisplay.renderBitmap->DrawBitmapTransparent(ENull, mBackground, rect, centerX, centerY);
}

void GSplashPlayfield::Animate() {
  // -1 means stop!
  if (mNumParticlesMoved > -1) {
    mNumParticlesMoved = 0;

    for (int i = 0; i < mNumParticles - 1; i++) {
      // Don't Animate
      if (mParticles[i].color == 254) {
        continue;
      }


      if (mParticles[i].screen_y < mParticles[i].y) {
        mParticles[i].screen_y += 2.5;
        mNumParticlesMoved++;

        if (mParticles[i].screen_y > mParticles[i].y) {
          mParticles[i].screen_y = mParticles[i].y;
        }
      }

    }
  }

  if (mNumParticlesMoved == 0) {
    mNumParticlesMoved = -1;  // Stop moving particles
    mAnimatedColorValue = 90;


  }

  if (mNumParticlesMoved == -1 && mAnimatedColorValue > 0) {
    mAnimatedColorValue -= 1;

    if (mAnimatedColorValue < 0) {
      mAnimatedColorValue = 0;
    }

    if (mAnimatedColorValue > 0) {
      for (TInt color = 0; color < 26; color++) {
        TRGB c = mSavedPalette[color];

        TRGB newC;

        TUint16 red =  c.r + (TUint8)mAnimatedColorValue;
        newC.r =   (TUint8)((red > 0xFF) ? 0xFF :  red);

        TUint16 green =  c.g + (TUint8)mAnimatedColorValue;
        newC.g =  (TUint8)((green > 0xFF) ? 0xFF : green);

        TUint16 blue =  c.b + (TUint8)mAnimatedColorValue;
        newC.b =  (TUint8)((blue > 0xFF) ? 0xFF : blue);

        gDisplay.SetColor(color, newC);
      }
    }

  }




}
