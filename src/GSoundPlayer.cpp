#include "Game.h"
#include "GSoundPlayer.h"
#include "GResources.h"
#include "Memory.h"

#define DISABLE_AUDIO
#undef DISABLE_AUDIO


GSoundPlayer gSoundPlayer;

static const TUint16 effectsList[] = {
  SFX_EMPTY_WAV,
  SFX_MENU_NAV_UP_WAV,
  SFX_MENU_NAV_DOWN_WAV,
  SFX_MENU_IN_WAV,
  SFX_MENU_OUT_WAV,
  SFX_MENU_OPTION_SELECT_WAV,
  SFX_SAVE_GAME_WAV,
  SFX_PLAYER_SLASH_WAV,
  SFX_PLAYER_QUAFF_HEALTH_POTION_WAV,
  SFX_PLAYER_QUAFF_WATER_SPELL_WAV,
  SFX_PLAYER_QUAFF_EARTH_SPELL_WAV,
  SFX_PLAYER_QUAFF_FIRE_SPELL_WAV,
  SFX_PLAYER_QUAFF_ENERGY_SPELL_WAV,
  SFX_PLAYER_TAKE_DAMAGE_WAV,
  SFX_ENEMY_DEATH_WAV,
  SFX_ENEMY_TAKE_DAMAGE_WAV,
  SFX_PLAYFIELD_SPIKE_RAISE_WAV,
};

static TUint16 FindSfxNumber(TUint16 aSfxFile) {

  for (size_t i = 0; i < sizeof(effectsList); i++) {
    if (aSfxFile == effectsList[i]) {
      return (TUint16) i; // Should not go above UINT16_MAX
    }
  }

  return UINT16_MAX;
}

static const TUint16 allSongs[] = {
  EMPTYSONG_XM,
  UNDER_WATER_XM,
  CYBERPUNK_XM,
  COUNTRYSIDE_XM,
  MAIN_MENU_XM,
  SPAAACE_XM,
  GLACIAL_MOUNTAINS_XM,
  GAMEOVER_XM,
  UNDERWATERFANTASY_XM,
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

  SetMusicVolume(gOptions->music);
  SetEffectsVolume(gOptions->sfx);
  MuteMusic(gOptions->muted);
}

TBool GSoundPlayer::PlayMusic(TInt16 aResourceId) {
  aResourceId = EMPTYSONG_XM;
  TBool music = BSoundPlayer::PlayMusic(aResourceId);
//  printf("%s %i\n", __PRETTY_FUNCTION__, aResourceId);
  // BSoundPlayer::PlayMusic un-mutes the music
  // We have to re-mute it in case of mute == true

  SetMusicVolume(gOptions->music);
  SetEffectsVolume(gOptions->sfx);
  MuteMusic(gOptions->muted);

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
  SetMusicVolume(.15);
  SetEffectsVolume(.15);
  return ETrue;
}



BRaw *GSoundPlayer::LoadEffectResource(TUint16 aResourceId, TInt16 aSlotNumber) {
  gResourceManager.LoadRaw(aResourceId, SFX1_SLOT + aSlotNumber);
  return gResourceManager.GetRaw(SFX1_SLOT + aSlotNumber);
}







void GSoundPlayer::SfxOptionSelect() {
  PlaySfx(FindSfxNumber(SFX_MENU_OPTION_SELECT_WAV));
}

void GSoundPlayer::SfxMenuNavDown() {
  PlaySfx(FindSfxNumber(SFX_MENU_NAV_DOWN_WAV));
}

void GSoundPlayer::SfxMenuNavUp() {
  PlaySfx(FindSfxNumber(SFX_MENU_NAV_UP_WAV));
}

void GSoundPlayer::SfxMenuIn() {
  PlaySfx(FindSfxNumber(SFX_MENU_IN_WAV));
}

void GSoundPlayer::SfxMenuOut() {
  PlaySfx(FindSfxNumber(SFX_MENU_OUT_WAV));
}


void GSoundPlayer::SfxSaveGame() {
  PlaySfx(FindSfxNumber(SFX_SAVE_GAME_WAV));
}

void GSoundPlayer::SfxStartGame() {
  SfxSaveGame();
}

// SFX Player //
void GSoundPlayer::SfxPlayerSlash(){
  PlaySfx(FindSfxNumber(SFX_PLAYER_SLASH_WAV));
}

void GSoundPlayer::SfxPlayerQuaffHealthPotion() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV));
}
void GSoundPlayer::SfxPlayerQuaffWaterSpell() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_QUAFF_WATER_SPELL_WAV));
}

void GSoundPlayer::SfxPlayerQuaffEarthSpell() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_QUAFF_EARTH_SPELL_WAV));
}
void GSoundPlayer::SfxPlayerQuaffFireSpell() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_QUAFF_FIRE_SPELL_WAV));
}
void GSoundPlayer::SfxPlayerQuaffEnergySpell() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_QUAFF_ENERGY_SPELL_WAV));
}

void GSoundPlayer::SfxPlayerTakeDamage() {
  PlaySfx(FindSfxNumber(SFX_PLAYER_TAKE_DAMAGE_WAV));
}


// SFX Enemy (general)
void GSoundPlayer::SfxEnemyDeath() {
  PlaySfx(FindSfxNumber(SFX_ENEMY_DEATH_WAV));
}
void GSoundPlayer::SfxEnemyTakeDamage() {
  PlaySfx(FindSfxNumber(SFX_ENEMY_TAKE_DAMAGE_WAV));
}

// SFX Playfield

void GSoundPlayer::SfxPlayfieldSpikeRaise() {
  PlaySfx(FindSfxNumber(SFX_PLAYFIELD_SPIKE_RAISE_WAV));
}