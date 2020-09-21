#ifndef MODITE_ITEMS_H
#define MODITE_ITEMS_H

#include "GGamePlayfield.h"
#include "GResources.h"

const TUint16 ITEM_EMPTY = ATTR_EMPTY;
const TUint16 ITEM_WATER_SPELLBOOK = ATTR_BLUE_SPELLBOOK;
const TUint16 ITEM_FIRE_SPELLBOOK = ATTR_RED_SPELLBOOK;
const TUint16 ITEM_EARTH_SPELLBOOK = ATTR_GREEN_SPELLBOOK;
const TUint16 ITEM_ENERGY_SPELLBOOK = ATTR_YELLOW_SPELLBOOK;
const TUint16 ITEM_SILVER_KEY = ATTR_SILVER_KEY;
const TUint16 ITEM_GOLD_KEY = ATTR_GOLD_KEY;
const TUint16 ITEM_WATER_RING = ATTR_BLUE_RING;
const TUint16 ITEM_FIRE_RING = ATTR_RED_RING;
const TUint16 ITEM_EARTH_RING = ATTR_GREEN_RING;
const TUint16 ITEM_ENERGY_RING = ATTR_YELLOW_RING;
const TUint16 ITEM_BOOTS = ATTR_BOOTS;
const TUint16 ITEM_GLOVES = ATTR_GLOVES;
const TUint16 ITEM_SWORD = ATTR_SWORD;
const TUint16 ITEM_WATER_AMULET = ATTR_BLUE_AMULET;
const TUint16 ITEM_FIRE_AMULET = ATTR_RED_AMULET;
const TUint16 ITEM_EARTH_AMULET = ATTR_GREEN_AMULET;
const TUint16 ITEM_ENERGY_AMULET = ATTR_YELLOW_AMULET;
const TUint16 ITEM_RED_POTION1 = ATTR_RED_POTION1;
const TUint16 ITEM_HEART = ATTR_HEART;
const TUint16 ITEM_RED_POTION2 = ATTR_RED_POTION2;
const TUint16 ITEM_BLUE_POTION1 = ATTR_BLUE_POTION1;
const TUint16 ITEM_BLUE_POTION2 = ATTR_BLUE_POTION2;
const TUint16 ITEM_EMPTY_BOTTLE = ATTR_EMPTY_BOTTLE;    // empty bottle
const TUint16 ITEM_BLUE_BOTTLE1 = ATTR_BLUE_BOTTLE1;   // 25% full
const TUint16 ITEM_BLUE_BOTTLE2 = ATTR_BLUE_BOTTLE2;   // 50% full
const TUint16 ITEM_BLUE_BOTTLE3 = ATTR_BLUE_BOTTLE3;   // 75% full
const TUint16 ITEM_BLUE_BOTTLE4 = ATTR_BLUE_BOTTLE4;   // 100% full
const TUint16 ITEM_RED_BOTTLE1 = ATTR_RED_BOTTLE1; // 25% full
const TUint16 ITEM_RED_BOTTLE2 = ATTR_RED_BOTTLE2; // 50% full
const TUint16 ITEM_RED_BOTTLE3 = ATTR_RED_BOTTLE3; // 75% full
const TUint16 ITEM_RED_BOTTLE4 = ATTR_RED_BOTTLE4; // 100% full

static const TInt items[] = {
  0, // 0 = no item
  IMG_BLUE_SPELLBOOK,
  IMG_RED_SPELLBOOK,
  IMG_GREEN_SPELLBOOK,
  IMG_YELLOW_SPELLBOOK,
  IMG_SILVER_KEY,
  IMG_GOLD_KEY,
  IMG_BLUE_RING,
  IMG_RED_RING,
  IMG_GREEN_RING,
  IMG_YELLOW_RING,
  IMG_BOOTS,
  IMG_GLOVES,
  IMG_SWORD,
  IMG_BLUE_AMULET,
  IMG_RED_AMULET,
  IMG_GREEN_AMULET,
  IMG_YELLOW_AMULET,
  IMG_GREEN_POTION1,
  IMG_GREEN_POTION2,
  IMG_BLUE_POTION1,
  IMG_BLUE_POTION2,
  IMG_HEART,
  IMG_EMPTY_BOTTLE,
  IMG_BLUE_BOTTLE1,
  IMG_BLUE_BOTTLE2,
  IMG_BLUE_BOTTLE3,
  IMG_BLUE_BOTTLE4,
  IMG_GREEN_BOTTLE1,
  IMG_GREEN_BOTTLE2,
  IMG_GREEN_BOTTLE3,
  IMG_GREEN_BOTTLE4,
};

static const char *itemNames[] = {
  "INVALID ITEM",
  "Water Spell",
  "Fire Spell",
  "Earth Spell",
  "Energy Spell",
  "Silver Key",
  "Gold Key",
  "Water Ring",
  "Fire Ring",
  "Earth Ring",
  "Energy Ring",
  "Boots",
  "Gloves",
  "Sword",
  "Water Amulet",
  "Fire Amulet",
  "Earth Amulet",
  "Energy Amulet",
  "Small Health Potion",
  "Large Health Potion",
  "Small Mana Potion",
  "Large Mana Potion",
  "Heart",
  "", // Not used "Empty Bottle",
  "", // Not used "Magic Bottle 25%",
  "", // Not used "Magic Bottle 50%",
  "", // Not used "Magic Bottle 75%",
  "", // Not used "Health Bottle Full",
  "", // Not used "Health Bottle 25%",
  "", // Not used "Health Bottle 50%",
  "", // Not used "Health Bottle 75%",
  "", // Not used "Health Bottle Full",
};

#endif //MODITE_ITEMS_H
