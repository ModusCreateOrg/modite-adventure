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
BITMAP title.bmp


#### Main_Menu
PATH resources/main_menu/
BITMAP sky.bmp
BITMAP mountains.bmp
BITMAP near_trees.bmp
BITMAP moon_with_logo.bmp
BITMAP walking_path.bmp
BITMAP logo.bmp

#### Ending
PATH resources/ending/
BITMAP modus_labs_logo_white.bmp
BITMAP sun_with_logo.bmp
#
### HERO
#
PATH resources/sprites/hero/
SPRITESHEET 64x64 chara_hero.bmp
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
SPRITESHEET 32x32 projectile_arrow.bmp
SPRITESHEET 32x32 projectile_crystal.bmp
SPRITESHEET 64x64 enemy_death.bmp
SPRITESHEET 64x64 crystal.bmp

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
### WIZARD BOSSES
#
PATH resources/sprites/wizards
SPRITESHEET 64x64 earth_wizard.bmp
SPRITESHEET 32x32 earth_projectile.bmp
SPRITESHEET 32x32 earth_pillar.bmp

SPRITESHEET 64x64 energy_wizard.bmp
SPRITESHEET 32x32 energy_projectile.bmp
SPRITESHEET 32x32 energy_pillar.bmp

SPRITESHEET 64x64 fire_wizard.bmp
SPRITESHEET 32x32 fire_projectile.bmp
SPRITESHEET 32x32 fire_pillar.bmp

SPRITESHEET 64x64 water_wizard.bmp
SPRITESHEET 32x32 water_projectile.bmp
SPRITESHEET 32x32 water_pillar.bmp

#
### FINAL BOSS
#
PATH resources/sprites/final_boss
SPRITESHEET 128x128 draganos.bmp

#
### TILEMAPS
#

## OVERWORLD LEVELS
PATH resources/tilemaps/overworld
TMX overworld.txt



## OVERWORLD 0 Dungeons
PATH resources/tilemaps/OW1/OW1_DGN1_256
TMX OW1_DGN1_256.txt
PATH resources/tilemaps/OW1/OW1_DGN2_512
TMX OW1_DGN2_512.txt
PATH resources/tilemaps/OW1/OW1_DGN3_768
TMX OW1_DGN3_768.txt
PATH resources/tilemaps/OW1/OW1_DGN4_1024
TMX OW1_DGN4_1024.txt

## OVERWORLD 1 Dungeons
PATH resources/tilemaps/OW2,3/OW2_DGN1_1280
TMX OW2_DGN1_1280.txt
PATH resources/tilemaps/OW2,3/OW2_DGN2_1536
TMX OW2_DGN2_1536.txt
PATH resources/tilemaps/OW2,3/OW2_DGN3_1792
TMX OW2_DGN3_1792.txt
PATH resources/tilemaps/OW2,3/OW2_DGN4_2048
TMX OW2_DGN4_2048.txt
PATH resources/tilemaps/OW2,3/OW2_DGN5_9472
TMX OW2_DGN5_9472.txt

## OVERWORLD 2 Dungeons
PATH resources/tilemaps/OW2,3/OW3_DGN1_2304
TMX OW3_DGN1_2304.txt
PATH resources/tilemaps/OW2,3/OW3_DGN2_2560
TMX OW3_DGN2_2560.txt
PATH resources/tilemaps/OW2,3/OW3_DGN3_2816
TMX OW3_DGN3_2816.txt
PATH resources/tilemaps/OW2,3/OW3_DGN4_3072
TMX OW3_DGN4_3072.txt



## OVERWORLD 3 Dungeons
PATH resources/tilemaps/OW4,6/OW4_DGN1_3328
TMX OW4_DGN1_3328.txt
PATH resources/tilemaps/OW4,6/OW4_DGN2_3584
TMX OW4_DGN2_3584.txt
PATH resources/tilemaps/OW4,6/OW4_DGN3_3840
TMX OW4_DGN3_3840.txt
PATH resources/tilemaps/OW4,6/OW4_DGN4_4096
TMX OW4_DGN4_4096.txt

## OVERWORLD 4 Dungeons
PATH resources/tilemaps/OW5,7/OW5_DGN1_4352
TMX OW5_DGN1_4352.txt
PATH resources/tilemaps/OW5,7/OW5_DGN2_4608
TMX OW5_DGN2_4608.txt
PATH resources/tilemaps/OW5,7/OW5_DGN3_4864
TMX OW5_DGN3_4864.txt
PATH resources/tilemaps/OW5,7/OW5_DGN4_5120
TMX OW5_DGN4_5120.txt

## OVERWORLD 5 Dungeons
PATH resources/tilemaps/OW4,6/OW6_DGN1_5376
TMX OW6_DGN1_5376.txt
PATH resources/tilemaps/OW4,6/OW6_DGN2_5632
TMX OW6_DGN2_5632.txt
PATH resources/tilemaps/OW4,6/OW6_DGN3_5888
TMX OW6_DGN3_5888.txt
PATH resources/tilemaps/OW4,6/OW6_DGN4_6144
TMX OW6_DGN4_6144.txt

## OVERWORLD 6 Dungeons
PATH resources/tilemaps/OW5,7/OW7_DGN1_6400
TMX OW7_DGN1_6400.txt
PATH resources/tilemaps/OW5,7/OW7_DGN2_6656
TMX OW7_DGN2_6656.txt
PATH resources/tilemaps/OW5,7/OW7_DGN3_6912
TMX OW7_DGN3_6912.txt
PATH resources/tilemaps/OW5,7/OW7_DGN4_7168
TMX OW7_DGN4_7168.txt

## OVERWORLD 7 Dungeons
PATH resources/tilemaps/OW8,9/OW8_DGN1_7424
TMX OW8_DGN1_7424.txt
PATH resources/tilemaps/OW8,9/OW8_DGN2_7680
TMX OW8_DGN2_7680.txt
PATH resources/tilemaps/OW8,9/OW8_DGN3_7936
TMX OW8_DGN3_7936.txt
PATH resources/tilemaps/OW8,9/OW8_DGN4_8192
TMX OW8_DGN4_8192.txt

## OVERWORLD 8 Dungeons
PATH resources/tilemaps/OW8,9/OW9_DGN1_8448
TMX OW9_DGN1_8448.txt
PATH resources/tilemaps/OW8,9/OW9_DGN2_8704
TMX OW9_DGN2_8704.txt
PATH resources/tilemaps/OW8,9/OW9_DGN3_8960
TMX OW9_DGN3_8960.txt
PATH resources/tilemaps/OW8,9/OW9_DGN4_9216
TMX OW9_DGN4_9216.txt

# Dev Dungeons
# PATH resources/tilemaps/OWX_DevDGN_9472
# TMX OWX_DevDGN_9472.txt


#
### ENVIRONMENT/OBJECTS
#
PATH resources/tilemaps/tilesets/
SPRITESHEET 32x32 GLOBAL_OBJECT_LAYER.bmp

########### SOUND :: MUSIC ###########
PATH resources/music/
RAW EmptySong.xm
RAW Dungeon_Home.xm
RAW Dungeon1.xm
//RAW Dungeon2.xm
RAW Dungeon3.xm
RAW Dungeon4.xm
RAW Dungeon3.xm
RAW Dungeon5.xm
RAW Dungeon9.xm
RAW GameOver.xm
RAW Main_Menu.xm
RAW Overworld.xm
RAW Logo_Reveal.xm
RAW Boss_1.xm
RAW Boss_2.xm
RAW Boss_3.xm
RAW Ending_Theme.xm

########### SOUND :: SFX ###########
PATH resources/sound_effects/
RAW SFX_empty.wav
RAW SFX_menu_nav_down.wav
RAW SFX_chest_open.wav
RAW SFX_pot_take_damage.wav
RAW SFX_pot_destroyed.wav
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
RAW SFX_player_quaff_spell.wav
RAW SFX_player_take_damage.wav
RAW SFX_player_death.wav
RAW SFX_player_parry_projectile.wav
RAW SFX_player_level_up.wav
RAW SFX_enemy_attack.wav
RAW SFX_enemy_death.wav
RAW SFX_enemy_take_damage.wav
RAW SFX_enemy_heal.wav
RAW SFX_item_heart.wav
RAW SFX_item_pickup_generic.wav
RAW SFX_midboss_bounce_wall.wav
RAW SFX_midboss_charge_step.wav
RAW SFX_midboss_land.wav
RAW SFX_midboss_transform_in.wav
RAW SFX_midboss_transform_out.wav
RAW SFX_midboss_attack_energy.wav
RAW SFX_midboss_attack_fire.wav
RAW SFX_midboss_attack_water.wav
RAW SFX_midboss_attack_earth.wav
RAW SFX_door_open.wav
RAW SFX_door_open_group.wav
RAW SFX_tower_projectile_death.wav
RAW SFX_wizard_fire_pillar.wav
RAW SFX_wizard_fire_pillar_death.wav
RAW SFX_wizard_exploding.wav
RAW SFX_wizard_teleport.wav
