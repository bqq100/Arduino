#include "Return.h"

Return::Return( Setting* settings, uint8_t returnPin ): Equip( settings, returnPin ){
  settings->init( &RETURN_MAX_DISABLE_NAME[0], &RETURN_MAX_DISABLE_DESC[0], &RETURN_MAX_DISABLE_UNIT[0], 60 );
  maxDisableName_ = &RETURN_MAX_DISABLE_NAME[0];
}

void Return::check( float currentTime ){
  if ( getStatus() )
    equipOn();
  else
    equipOff();
}
