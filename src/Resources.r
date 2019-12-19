#
### FONTS
#
PATH resources/charset
BITMAP charset_8x8.bmp
BITMAP charset_16x16.bmp

#
### SPLASH, TITLE, ETC.
#
PATH resources/
BITMAP splash.bmp
BITMAP splash_sprites.bmp
BITMAP title.bmp
PALETTE test.pal


#
### HERO
#
PATH resources/sprites/
SPRITESHEET 64x64 chara_hero.bmp
SPRITESHEET 32x32 chara_hero_heal_effect.bmp
SPRITESHEET 32x32 chara_hero_spell_effect.bmp

#
### SPELLS
#
PATH resources/sprites/spells/
SPRITESHEET 64x64 spell_earth.bmp
SPRITESHEET 64x64 spell_electricity.bmp
SPRITESHEET 64x64 spell_fire.bmp
SPRITESHEET 64x64 spell_water.bmp

#
### ENEMIES
#
PATH resources/sprites/enemies
SPRITESHEET 64x64 chara_bat.bmp
SPRITESHEET 64x64 chara_goblin.bmp
SPRITESHEET 64x64 chara_goblin_sniper.bmp
SPRITESHEET 64x64 chara_orc.bmp
SPRITESHEET 64x64 chara_rat.bmp
SPRITESHEET 64x64 chara_slime.bmp
SPRITESHEET 64x64 chara_spider.bmp
SPRITESHEET 64x64 chara_troll.bmp
# SPRITESHEET 64x64 projectile_arrow.bmp
SPRITESHEET 64x64 enemy_death.bmp

#
### MID BOSSES
#
PATH resources/sprites/mid_boss
SPRITESHEET 64x64 mid_boss_death_explosion.bmp
SPRITESHEET 128x128 mid_boss_earth_brown.bmp
SPRITESHEET 32x32 mid_boss_earth_projectile.bmp
SPRITESHEET 128x128 mid_boss_energy.bmp
SPRITESHEET 32x32 mid_boss_energy_projectile.bmp
SPRITESHEET 128x128 mid_boss_fire.bmp
SPRITESHEET 32x32 mid_boss_fire_projectile.bmp
SPRITESHEET 128x128 mid_boss_water.bmp
SPRITESHEET 32x32 mid_boss_water_projectile.bmp

#
### TILEMAPS
#
# PATH resources/tilemaps/DevDungeon_0
# TILEMAP DevDungeon_0.filelist.txt
PATH resources/tilemaps/overworld/
TMX overworld.txt

PATH resources/tilemaps/DGN1
TMX DGN1.txt

PATH resources/tilemaps/DGN2
TMX DGN2.txt

PATH resources/tilemaps/DGN3
TMX DGN3.txt


#
# PATH resources/tilemaps-new/256
# TMX overworld.txt
# PATH resources/tilemaps-new/257
# TMX 257.txt
# PATH resources/tilemaps-new/258
# TMX 258.txt
# PATH resources/tilemaps-new/259
# TMX 259.txt

#
### ENVIRONMENT/OBJECTS
#
PATH resources/tilemaps/tilesets/
SPRITESHEET 32x32 GLOBAL_OBJECT_LAYER.bmp

########### SOUND :: MUSIC ###########
PATH resources/music/
RAW EmptySong.xm
RAW Countryside.xm
RAW Cyberpunk.xm
RAW GameOver.xm
RAW Glacial_mountains.xm
RAW Main_Menu.xm
RAW Spaaace.xm
RAW UnderWaterFantasy.xm
RAW Under_Water.xm
RAW EnterCredits.xm

########### SOUND :: SFX ###########
PATH resources/sound_effects/
RAW SFX_empty.wav
RAW SFX_menu_nav_down.wav
RAW SFX_menu_nav_up.wav
RAW SFX_menu_in.wav
RAW SFX_menu_out.wav
RAW SFX_menu_option_select.wav
RAW SFX_save_game.wav
RAW SFX_player_slash.wav
RAW SFX_player_quaff_health_potion.wav
RAW SFX_player_quaff_water_spell.wav
RAW SFX_player_quaff_earth_spell.wav
RAW SFX_player_quaff_fire_spell.wav
RAW SFX_player_quaff_energy_spell.wav
RAW SFX_player_take_damage.wav
RAW SFX_enemy_death.wav
RAW SFX_enemy_take_damage.wav
RAW SFX_playfield_spike_raise.wav

