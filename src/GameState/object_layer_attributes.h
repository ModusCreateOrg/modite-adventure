#ifndef OBJECT_LAYER_ATTRIBUTES_H
#define OBJECT_LAYER_ATTRIBUTES_H

#include <BTypes.h>

// Formation of Pro Motion OBJECT_ATTRIBUTE layer CODEs.
// We have 16 bites of attributes.  This struct + union defines how the bits are assigned and used.
//
// See tools/object_attribute.cpp - the executable takes a group, order, and item and prints out the code.
struct OBJECT_ATTRIBUTE {
  int group : 4;  // 4 bits of group to assocate things together
                  // next 8 bits are either order or item number, accessed via attribute.order or attribute.item
  int order : 5;  // see order bits below
  int state : 2;  // desired state for operation of 3-way switches
  int unused : 4; // unused bits
  int flag : 1;
};

const TUint8 OA_GROUP_NONE = 0;  // thing does not belong to any group
const TUint8 OA_GROUP_ITEM = 15; // thing requires an item to be operated

// ORDER
//
// If you have 3 switches that are used as a combination to open a door,
// you would use these bits to define the combination.  That is, if you want
// switch 1, then 3, then 2 to be the combination, you would set OA_ORDER_FIRST on switch 1,
// OA_ORDER_SECOND on switch 3, and OA_ORDER_THIRD on switch 2.  If you don't care about the
// order, use OA_ORDER_ANY.
//
// The swtiches and the door(s) would be in the same OA_GROUP.
const TUint8 OA_ORDER_NONE = 0;        // this thing should not be operated
const TUint8 OA_ORDER_FIRST = 1;       // this thing must be operated first
const TUint8 OA_ORDER_SECOND = 2;      // this thing must be operated second
const TUint8 OA_ORDER_THIRD = 3;       // this thing must be operated third
const TUint8 OA_ORDER_FOURTH = 4;      // this thing must be operated fourth
const TUint8 OA_ORDER_FIFTH = 5;       // this thing must be operated fifth
const TUint8 OA_ORDER_SIXTH = 6;       // this thing must be operated sixth
const TUint8 OA_ORDER_SEVENTH = 7;     // this thing must be operated seventh
const TUint8 OA_ORDER_EIGHTH = 8;      // this thing must be operated eighth
const TUint8 OA_ORDER_NINTH = 9;       // this thing must be operated ninth
const TUint8 OA_ORDER_TENTH = 10;      // this thing must be operated tenth
const TUint8 OA_ORDER_ELEVENTH = 11;   // this thing must be operated eleventh
const TUint8 OA_ORDER_TWELFTH = 12;    // this thing must be operated twelfth
const TUint8 OA_ORDER_THIRTEENTH = 13; // this thing must be operated thirteenth
const TUint8 OA_ORDER_FOURTEENTH = 14; // this thing must be operated fourteenth
const TUint8 OA_ORDER_ANY = 15;        // this thing may be operated in any order

const TUint8 OA_STATE_NONE = 0;   // don't care or NONE
const TUint8 OA_STATE_LEFT = 1;   // switch must be in the left position
const TUint8 OA_STATE_CENTER = 2; // switch must be in the center position
const TUint8 OA_STATE_RIGHT = 3;  // switch must be in the right position

// handy macro for generating the CODE to be used for the tile in Pro Motion
static inline TUint16 MAKE_OBJECT_ATTRIBUTE(TInt8 group, TInt8 order, TInt8 state = 0) {
  struct OBJECT_ATTRIBUTE oa;
  oa.group = group;
  oa.order = order;
  oa.state = state;
  oa.flag = 1;
  TUint16 *ptr = (TUint16 *)&oa;
  return *ptr;
}

#endif
