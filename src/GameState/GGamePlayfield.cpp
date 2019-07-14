#include "GGamePlayfield.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTilesSlot, TUint16 aMapSlot, TUint16 aCodesSlot)
  : BMapPlayfield(aViewPort, aTilesSlot, aMapSlot, aCodesSlot) {

}

GGamePlayfield::~GGamePlayfield() {
  //
}