//
// Created by Jesus Garcia on 10/2/18.
//

#include "Game.h"
#include "GSoundPlayer.h"
#include "GResources.h"
#include "Memory.h"

#define DISABLE_AUDIO
#undef DISABLE_AUDIO


GSoundPlayer gSoundPlayer;

static const TUint16 effectsList[] = {
  SFX_OPTION_SELECT_WAV,

};

static const TUint16 allSongs[] = {
  EMPTYSONG_XM,
  ENTERCREDITS_XM
};


void GSoundPlayer::Init(TUint8 aNumberFxChannels) {
  mMaxSongs = sizeof(allSongs) / sizeof(TUint16);
  mMaxEffects = sizeof(effectsList) / sizeof(TUint16);

  BSoundPlayer::Init(aNumberFxChannels, mMaxEffects);

  mSongSlots = (SongSlot *)AllocMem(sizeof(SongSlot) * mMaxSongs, MEMF_SLOW);

  for (TUint8 i = 0; i < mMaxSongs; i++) {
    auto *slot = (SongSlot *)AllocMem(sizeof(SongSlot), MEMF_SLOW);

    slot->mResourceNumber = allSongs[i];
    slot->mSlotNumber = SONG0_SLOT + i;

    gResourceManager.LoadRaw(allSongs[i], slot->mSlotNumber);
    slot->mRaw = gResourceManager.GetRaw(slot->mSlotNumber);

    mSongSlots[i] = *slot;
    FreeMem(slot);
  }


  PlayMusic(EMPTYSONG_XM);

// TODO @mtintiuc
//  SetMusicVolume(gOptions->music);
//  SetEffectsVolume(gOptions->sfx);
//  MuteMusic(gOptions->muted);
}

TBool GSoundPlayer::PlayMusic(TInt16 aResourceId) {
  TBool music = BSoundPlayer::PlayMusic(aResourceId);
//  printf("%s %i\n", __PRETTY_FUNCTION__, aResourceId);
  // BSoundPlayer::PlayMusic un-mutes the music
  // We have to re-mute it in case of mute == true
//TODO: @mtintiuc
//  MuteMusic(gOptions->muted);

  return music;
}

TBool GSoundPlayer::LoadSongSlot(TInt16 aResourceId) {

  for (TUint8 i = 0; i < mMaxSongs; i++) {
    if (mSongSlots[i].mResourceNumber == aResourceId) {
      return LoadSong(mSongSlots[i].mRaw);
    }
  }

  printf("WARNING :: Could not find song %i\n", aResourceId);

  return false;
}


TBool GSoundPlayer::LoadEffects() {
  for (TUint8 i = 0; i < mMaxEffects; i++) {
    LoadEffect(effectsList[i], i);
  }

// TODO: @mtintiuc
//  SetMusicVolume(gOptions->music);
//  SetEffectsVolume(gOptions->sfx);
  return ETrue;
}



BRaw *GSoundPlayer::LoadEffectResource(TUint16 aResourceId, TInt16 aSlotNumber) {
  gResourceManager.LoadRaw(aResourceId, SFX1_SLOT + aSlotNumber);
  return gResourceManager.GetRaw(SFX1_SLOT + aSlotNumber);
}


