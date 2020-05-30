#ifndef GSOUNDPLAYER_H
#define GSOUNDPLAYER_H

#include "BSoundEngine.h"

#include "Resources.h"


class GSoundPlayer : public BBase {
private:
    TUint8 mNumberFxChannels;
    TUint8 mNumberFxSlots;

    TUint8 mMaxSongs;
    TUint8 mMaxEffects;
    SongSlot *mSongSlots; // Used to store number of slots

public:
  ~GSoundPlayer();
  void Init(TUint8 aNumberFxChannels);

  BRaw *FindRawSongFileById(TInt16 aResourceId);
  TBool LoadEffects();
  TBool PlayMusic(TInt16 aResourceId);
  void MuteMusic(TBool aMuted);

  void TriggerSfx(TUint16 aSfxNumber);
  TBool SetMusicVolume(TFloat aPercent) {
    return soundEngine.SetMusicVolume(aPercent);
  }
  TBool SetEffectsVolume(TFloat aPercent) {
    return soundEngine.SetEffectsVolume(aPercent);
  }
  // SFX Methods //

  void SfxSaveGame();
  void SfxStartGame();
  void SfxMenuNavDown();
  void SfxMenuNavUp();
  void SfxMenuIn();
  void SfxMenuOut();

  void SfxOptionSelect();

  // SFX PLAYER //

  void SfxPlayerSlash();

  void SfxItemHeart();
  void SfxItemPickupGeneric();

};


extern GSoundPlayer gSoundPlayer;

#endif //GSOUNDPLAYER_H
