#include "Return.h"

Return::Return( Setting* settings, Clock* clock, uint8_t returnPin ): Equip( settings, clock, returnPin ){
  settings->init( &RETURN_MAX_DISABLE_NAME[0], &RETURN_MAX_DISABLE_DESC[0], &RETURN_MAX_DISABLE_UNIT[0], 1440 );
  maxDisableName_ = &RETURN_MAX_DISABLE_NAME[0];
}

void Return::check(){
  if ( forceOn_ )
    equipOn();
  else if ( getStatus() )
    equipOn();
  else
    equipOff();
}

void Return::check( bool lowWaterAlarm ){
  if( lowWaterAlarm )
    equipOff();
  else
    check();
}
