#include "Game.h"
#include "GSoundPlayer.h"
#include "GResources.h"
#include "Memory.h"

//#define DISABLE_AUDIO
//#undef DISABLE_AUDIO


GSoundPlayer gSoundPlayer;

static const TUint16 effectsList[] = {
  SFX_EMPTY_WAV,
  SFX_CHEST_OPEN_WAV,
  SFX_POT_TAKE_DAMAGE_WAV,
  SFX_POT_DESTROYED_WAV,
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
  SFX_PLAYER_QUAFF_SPELL_WAV,
  SFX_PLAYER_TAKE_DAMAGE_WAV,
  SFX_PLAYER_DEATH_WAV,
  SFX_PLAYER_PARRY_PROJECTILE_WAV,
  SFX_PLAYER_LEVEL_UP_WAV,
  SFX_PLAYER_LOW_HEALTH_WAV,
  SFX_ENEMY_ATTACK_WAV,
  SFX_ENEMY_DEATH_WAV,
  SFX_ENEMY_HEAL_WAV,
  SFX_ENEMY_TAKE_DAMAGE_WAV,
  SFX_ITEM_HEART_WAV,
  SFX_ITEM_PICKUP_GENERIC_WAV,
  SFX_MIDBOSS_BOUNCE_WALL_WAV,
  SFX_MIDBOSS_CHARGE_STEP_WAV,
  SFX_MIDBOSS_LAND_WAV,
  SFX_MIDBOSS_TRANSFORM_IN_WAV,
  SFX_MIDBOSS_TRANSFORM_OUT_WAV,
  SFX_MIDBOSS_ATTACK_ENERGY_WAV,
  SFX_MIDBOSS_ATTACK_FIRE_WAV,
  SFX_MIDBOSS_ATTACK_WATER_WAV,
  SFX_MIDBOSS_ATTACK_EARTH_WAV,
  SFX_DOOR_OPEN_WAV,
  SFX_DOOR_OPEN_GROUP_WAV,
  SFX_WIZARD_FIRE_PILLAR_WAV,
  SFX_WIZARD_FIRE_PILLAR_DEATH_WAV,
  SFX_WIZARD_EXPLODING_WAV,
  SFX_WIZARD_TELEPORT_WAV,
  SFX_TOWER_PROJECTILE_DEATH_WAV
};

static TUint16 FindSfxNumber(TUint16 aSfxFile) {

  for (size_t i = 0; i < sizeof(effectsList); i++) {
    if (aSfxFile == effectsList[i]) {
      return (TUint16) i; // Should not go above UINT16_MAX
    }
  }
  printf("WARNING :: Could not find SFX for ID %i\n", aSfxFile);


  return UINT16_MAX;
}

static const TUint16 allSongs[] = {
  EMPTYSONG_XM,
  LOGO_REVEAL_XM,
  MAIN_MENU_XM,
  GAMEOVER_XM,
  OVERWORLD_XM,
  BOSS_1_XM,
  BOSS_2_XM,
  BOSS_3_XM,
  DUNGEON_HOME_XM,
  DUNGEON1_XM,
  DUNGEON2_XM,
  DUNGEON3_XM,
  DUNGEON4_XM,
  DUNGEON5_XM,
  DUNGEON6_XM,
  DUNGEON7_XM,
//  DUNGEON8_XM,
  DUNGEON9_XM,
  ENDING_THEME_XM
};

GSoundPlayer::~GSoundPlayer() {
  // Release songs memory
  FreeMem(mSongSlots);
}

void GSoundPlayer::Init(TUint8 aNumberFxChannels) {
#ifdef DISABLE_AUDIO
  return;
#endif
  mMaxSongs = sizeof(allSongs) / sizeof(TUint16);
  mMaxEffects = sizeof(effectsList) / sizeof(TUint16);

  soundEngine.InitAudioEngine(aNumberFxChannels, mMaxEffects);

//  SDL_ClearError();
  LoadEffects();
//  SDL_ClearError();



  mSongSlots = (SongSlot *)AllocMem(sizeof(SongSlot) * mMaxSongs, MEMF_SLOW);

//  for (TUint8 i = 0; i < mMaxSongs; i++) {
//    auto *slot = (SongSlot *)AllocMem(sizeof(SongSlot), MEMF_SLOW);
//
//    slot->mResourceNumber = allSongs[i];
//    slot->mSlotNumber = SONG0_SLOT + i;
//
//    gResourceManager.LoadRaw(allSongs[i], slot->mSlotNumber);
//    slot->mRaw = gResourceManager.GetRaw(slot->mSlotNumber);
//
//    mSongSlots[i] = *slot;
//    FreeMem(slot);
//  }
  PlayMusic(EMPTYSONG_XM);


  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);
}

TBool GSoundPlayer::PlayMusic(TInt16 aResourceId) {
#ifdef DISABLE_AUDIO
  return ETrue;
#endif
  //  aResourceId = EMPTYSONG_XM;
//  printf("%s %i\n", __PRETTY_FUNCTION__, aResourceId);

  BRaw *songToLoad = FindRawSongFileById(aResourceId);

  TBool music = soundEngine.PlayMusic(songToLoad, aResourceId);
//   BSoundEngine::PlayMusic un-mutes the music
// We have to re-mute it in case of mute == true

  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);

  return music;
}


BRaw *GSoundPlayer::FindRawSongFileById(TInt16 aResourceId) {

  for (TUint8 i = 0; i < mMaxSongs; i++) {
    if (allSongs[i] == aResourceId) {
      if (gResourceManager.GetRaw(SONG0_SLOT)) {
        gResourceManager.ReleaseRawSlot(SONG0_SLOT);
      }
      gResourceManager.LoadRaw(allSongs[i], SONG0_SLOT);
      return gResourceManager.GetRaw(SONG0_SLOT);;
    }
  }

  printf("WARNING :: Could not find song %i\n", aResourceId);

  return ENull;
}


TBool GSoundPlayer::LoadEffects() {
#ifdef DISABLE_AUDIO
  return ETrue;
#endif
  for (TUint8 index = 0; index < mMaxEffects; index++) {
    soundEngine.LoadEffect(index, effectsList[index], SFX1_SLOT + index);
//    return ETrue;
  }
  return ETrue;
}


void  GSoundPlayer::MuteMusic(TBool aMuted) {
  return soundEngine.MuteMusic(aMuted);
}



void GSoundPlayer::TriggerSfx(TUint16 aSfxNumber, TInt8 aChannel) {
#ifdef DISABLE_AUDIO
  return;
#endif
  soundEngine.PlaySfx(FindSfxNumber(aSfxNumber), aChannel);
}


void GSoundPlayer::SfxOptionSelect() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_OPTION_SELECT_WAV), 1);
}

void GSoundPlayer::SfxMenuNavDown() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_NAV_DOWN_WAV), 1);
}

void GSoundPlayer::SfxMenuNavUp() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_NAV_UP_WAV), 1);
}

void GSoundPlayer::SfxMenuIn() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_IN_WAV), 1);
}

void GSoundPlayer::SfxMenuOut() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_OUT_WAV), 1);
}


void GSoundPlayer::SfxSaveGame() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_SAVE_GAME_WAV), 1);
}

void GSoundPlayer::SfxStartGame() {
  SfxSaveGame();
}

// SFX Player //
void GSoundPlayer::SfxPlayerSlash(){
  soundEngine.PlaySfx(FindSfxNumber(SFX_PLAYER_SLASH_WAV), 1);
}


void GSoundPlayer::SfxItemHeart() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_ITEM_HEART_WAV), 2);
}
void GSoundPlayer::SfxItemPickupGeneric() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_ITEM_PICKUP_GENERIC_WAV), 3);
}