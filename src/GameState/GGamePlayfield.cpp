#include "GGamePlayfield.h"
#include "GResources.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId)
  : BMapPlayfield(aViewPort, aTileMapId, TILESET_SLOT, ETrue) {

  mTileMapId = aTileMapId;
  gDisplay.SetPalette(this->mTileset, 0, 128);
  for (TInt s = 0; s < 16; s++) {
    mGroupDone[s] = mGroupState[s] = EFalse;
  }
  mMosaic = 0;
}

GGamePlayfield::~GGamePlayfield() {
  //
}

void GGamePlayfield::Render() {
  mMosaic > 0 ? mMosaic-- : mMosaic++;
  TRect &rect = mViewPort->mRect;
  TUint8 *pixels = &gDisplay.renderBitmap->mPixels[0];

  TInt startX = TInt(mViewPort->mWorldX) % TILESIZE,
          startY = TInt(mViewPort->mWorldY) % TILESIZE;

  TInt offRow = TInt(mViewPort->mWorldY) / TILESIZE,
          offCol = TInt(mViewPort->mWorldX) / TILESIZE,
          tilesHigh = TInt(ceil(TFloat(rect.Height()) / TILESIZE)) + (startY ? 1 : 0),
          tilesWide = TInt(ceil(TFloat(rect.Width()) / TILESIZE)) + (startX ? 1 : 0);

  TInt tw = mTileMap->mTiles->Width(); // width of tileset bitmap

  TInt xx = rect.x1;
  for (TInt col = 0; col < tilesWide; col++) {
    TInt yy = rect.y1;
    const TInt offset = yy * SCREEN_WIDTH + xx;
    TUint8 *bm = &pixels[offset];
    for (TInt row = 0; row < tilesHigh; row++) {
      TInt h = MIN(SCREEN_HEIGHT - yy, row ? TILESIZE : TILESIZE - startY),
              w = MIN(SCREEN_WIDTH - xx, col ? TILESIZE : TILESIZE - startX);

      TUint8 *tile = mTileMap->TilePtr(row + offRow, col + offCol);
      if (row == 0) {
        tile = &tile[startY * tw];
      }
      if (col == 0) {
        tile = &tile[startX];
      }

      for (TInt y = 0; y < h; y++) {
        for(TInt i = 0; i < w; i++) {
          bm[i] = tile[i];
        }
        bm += SCREEN_WIDTH;
        tile += tw;
        yy++;
      }
    }
    xx += col ? TILESIZE : TILESIZE - startX;
  }

}

void GGamePlayfield::StartMosaicIn() {
  mMosaic = -100;
}

void GGamePlayfield::StartMosaicOut() {
  mMosaic = 100;
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
