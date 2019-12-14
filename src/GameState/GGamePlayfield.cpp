#include "GGamePlayfield.h"
#include "GResources.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId)
  : BMapPlayfield(aViewPort, aTileMapId, TILESET_SLOT, ETrue) {

  mTileMapId = aTileMapId;
  gDisplay.SetPalette(this->mTileset, 0, 128);
  for (TInt s = 0; s < 16; s++) {
    mGroupDone[s] = mGroupState[s] = EFalse;
  }
  mMosaicTimer = -FRAMES_PER_SECOND;
}

GGamePlayfield::~GGamePlayfield() {
  //
}

void GGamePlayfield::Render() {
  BMapPlayfield::Render();
  if (mMosaicTimer) {
    mMosaicTimer > 0 ? mMosaicTimer-- : mMosaicTimer++;

    const TInt mosaicWidth = (ABS(mMosaicTimer) * 10 / FRAMES_PER_SECOND) + 1;
    const TRect r = mViewPort->mRect;
    TUint8 *pixels = &gDisplay.renderBitmap->mPixels[0];

    for (TInt y = r.y1; y < SCREEN_HEIGHT; y++) {
      for (TInt x = r.x1; x < SCREEN_WIDTH; x++) {
        pixels[y * SCREEN_WIDTH + x] = pixels[(y - (y - r.y1) % mosaicWidth) * SCREEN_WIDTH + (x - (x - r.x1) % mosaicWidth)];
      }
    }
  }
}

void GGamePlayfield::StartMosaicIn() {
  mMosaicTimer = -FRAMES_PER_SECOND;
}

void GGamePlayfield::StartMosaicOut() {
  mMosaicTimer = FRAMES_PER_SECOND;
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
