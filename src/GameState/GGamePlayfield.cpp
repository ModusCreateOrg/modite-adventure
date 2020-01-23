#include "GGamePlayfield.h"
#include "GResources.h"
#include "GHud.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId)
  : BMapPlayfield(aViewPort, aTileMapId, TILESET_SLOT, ETrue) {

  mTileMapId = aTileMapId;
//  gDisplay.SetPalette(this->mTileset, 0, 128);
  for (TInt s = 0; s < 16; s++) {
    mGroupDone[s] = mGroupState[s] = EFalse;
  }
  mMosaicTimer = 0;

  GHud::SetColors();

  TRGB *source = gDisplay.renderBitmap->GetPalette();
  for (TInt color = 0; color < 255; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    gDisplay.SetColor(color, 0,0,0);
  }

}

GGamePlayfield::~GGamePlayfield() {
  //
}

void GGamePlayfield::Render() {
  BMapPlayfield::Render();
  if (mMosaicTimer) {
    TInt mosaicWidth = 1 + (mMosaicIn ? mMosaicTimer : (MOSAIC_DURATION - mMosaicTimer)) * MOSAIC_INTENSITY / MOSAIC_DURATION;
    TRect &r = mViewPort->mRect;
    TUint8 *pixels = &gDisplay.renderBitmap->mPixels[0];

    for (TInt y = r.y1; y < SCREEN_HEIGHT; y++) {
      for (TInt x = r.x1; x < SCREEN_WIDTH; x++) {
        pixels[y * SCREEN_WIDTH + x] = pixels[(y - (y - r.y1) % mosaicWidth) * SCREEN_WIDTH + (x - (x - r.x1) % mosaicWidth)];
      }
    }

//    var delta = max - min;
//    return function (val) {
//      return (val - min) / delta;
//    };
    TFloat pct = (((TFloat)mosaicWidth / 21) - 1) * -1;
    if (pct < 0 || pct == -0) {
      pct = 0;
    }

    printf("mosaicWidth %i, pct %f\n", mosaicWidth, pct);


    for (TInt color = 0; color < 255; color++) {
      TRGB c = mSavedPalette[color];

      TRGB newC;

      TUint16 red =  (TFloat)c.r * pct;
      newC.r = (TUint8)((red > 0xFF) ? 0xFF :  red);

      TUint16 green =  (TFloat)c.g * pct;
      newC.g = (TUint8)((green > 0xFF) ? 0xFF : green);

      TUint16 blue =  (TFloat)c.b * pct;
      newC.b = (TUint8)((blue > 0xFF) ? 0xFF : blue);

      gDisplay.SetColor(color, newC);
    }

    mMosaicTimer--;
  }
}

void GGamePlayfield::StartMosaicIn() {
  if (!mMosaicTimer) {
    GHud::SetColors();
    // Cache colors
    TRGB *source = gDisplay.renderBitmap->GetPalette();
    for (TInt color = 0; color < 255; color++) {
      TRGB c = source[color];
      mSavedPalette[color] = c;
      gDisplay.SetColor(color, 0,0,0);
    }

    mMosaicTimer = MOSAIC_DURATION;
    mMosaicIn = ETrue;
  }
}


void GGamePlayfield::StartMosaicOut() {
  if (!mMosaicTimer) {
    TRGB *source = gDisplay.renderBitmap->GetPalette();
    for (TInt color = 0; color < 255; color++) {
      TRGB c = source[color];
      mSavedPalette[color] = c;
    }
    mMosaicTimer = MOSAIC_DURATION;
    mMosaicIn = EFalse;
  }
}

void GGamePlayfield::DumpMap() {
  printf("MAP DATA\n");
  for (TInt row = 0; row < mMapHeight; row++) {
    for (TInt col = 0; col < mMapWidth; col++) {
      printf("%5d ", mMapData[row * mMapWidth + col] & 0xffff);
    }
    printf("\n");
  }
}

void GGamePlayfield::DumpMapAttributes() {
  printf("MAP ATTRIBUTES\n");
  for (TInt row = 0; row < mMapHeight; row++) {
    for (TInt col = 0; col < mMapWidth; col++) {
      printf("%5d ", (mMapData[row * mMapWidth + col] >> 16) & 0xffff);
    }
    printf("\n");
  }
}

void GGamePlayfield::Restore() {
  gResourceManager.ReleaseBitmapSlot(TILESET_SLOT);
  mTileMap = gResourceManager.LoadTileMap(mTileMapId, TILESET_SLOT);
}

void GGamePlayfield::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mTileMapId, sizeof(mTileMapId));
  aStream.Write(&mGroupDone[0], sizeof(TBool) * 16);
  aStream.Write(&mGroupState[0], sizeof(TBool) * 16);
  // object proram
  aStream.Write(&mObjectCount, sizeof(mObjectCount));
  aStream.Write(mObjectProgram, sizeof(BObjectProgram) * mObjectCount);
  printf("BMapPlayfield: %d Objects\n", mObjectCount);
  DumpObjectProgram();
}

void GGamePlayfield::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mTileMapId, sizeof(mTileMapId));
  aStream.Read(&mGroupDone[0], sizeof(TBool) * 16);
  aStream.Read(&mGroupState[0], sizeof(TBool) * 16);
  // object proram
  aStream.Read(&mObjectCount, sizeof(mObjectCount));
  mObjectProgram = new BObjectProgram[mObjectCount];
  aStream.Read(mObjectProgram, sizeof(BObjectProgram) * mObjectCount);
  printf("BMapPlayfield: %d Objects\n", mObjectCount);
  DumpObjectProgram();
}
