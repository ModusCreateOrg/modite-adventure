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
### ENVIRONMENT/OBJECTS
#
PATH resources/sprites/environment
SPRITESHEET 32x32 dungeon_tileset_objects.bmp

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
SPRITESHEET 64x64 projectile_arrow.bmp
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
PATH resources/tilemaps-new/DevDungeon_0
TMX DevDungeo_0.txt
PATH resources/tilemaps-new/Dungeon_1
TMX Dungeon_1.txt

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

