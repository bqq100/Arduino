#include "Light.h"

Light::Light( Setting* settings, uint8_t dimmerPin1, uint8_t dimmerPin2 ): Module( settings ){

  settings->init( &LIGHT_ON_HOUR_NAME[0]  , &LIGHT_ON_HOUR_DESC[0]  , 12 );
  settings->init( &LIGHT_ON_MINUTE_NAME[0], &LIGHT_ON_MINUTE_DESC[0], 0  );

  settings->init( &LIGHT_MAX_DISABLE_NAME[0], &LIGHT_MAX_DISABLE_DESC[0], &LIGHT_MAX_DISABLE_UNIT[0], 72  );
  settings->init( &LIGHT_RAMP_TIME_NAME[0]  , &LIGHT_RAMP_TIME_DESC[0]  , &LIGHT_RAMP_TIME_UNIT[0]  , 180 );
  settings->init( &LIGHT_PEAK_TIME_NAME[0]  , &LIGHT_PEAK_TIME_DESC[0]  , &LIGHT_PEAK_TIME_UNIT[0]  , 4   );
  settings->init( &LIGHT_MAX_CH1_NAME[0]    , &LIGHT_MAX_CH1_DESC[0]    , &LIGHT_MAX_CH1_UNIT[0]    , 100 );
  settings->init( &LIGHT_MAX_CH2_NAME[0]    , &LIGHT_MAX_CH2_DESC[0]    , &LIGHT_MAX_CH2_UNIT[0]    , 100 );

  maxDisableName_ = &LIGHT_MAX_DISABLE_NAME[0];
}

void Light::check( float currentTime ){
}

