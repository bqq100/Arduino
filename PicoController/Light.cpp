#include "Light.h"

Light::Light( Setting* settings, uint8_t dimmerPin1, uint8_t dimmerPin2 ): Module( settings ){

  settings->init( &LIGHT_ON_HOUR_NAME[0]  , &LIGHT_ON_HOUR_DESC[0]  , 12 );
  settings->init( &LIGHT_ON_MINUTE_NAME[0], &LIGHT_ON_MINUTE_DESC[0], 0  );

  settings->init( &LIGHT_MAX_DISABLE_NAME[0], &LIGHT_MAX_DISABLE_DESC[0], &LIGHT_MAX_DISABLE_UNIT[0], 72  );
  settings->init( &LIGHT_RAMP_TIME_NAME[0]  , &LIGHT_RAMP_TIME_DESC[0]  , &LIGHT_RAMP_TIME_UNIT[0]  , 180 );
  settings->init( &LIGHT_PEAK_TIME_NAME[0]  , &LIGHT_PEAK_TIME_DESC[0]  , &LIGHT_PEAK_TIME_UNIT[0]  , 4   );
  settings->init( &LIGHT_MAX_CH1_NAME[0]    , &LIGHT_MAX_CH1_DESC[0]    , &LIGHT_MAX_CH1_UNIT[0]    , 100 );
  settings->init( &LIGHT_MAX_CH2_NAME[0]    , &LIGHT_MAX_CH2_DESC[0]    , &LIGHT_MAX_CH2_UNIT[0]    , 100 );

  dimmerPin1_ = dimmerPin1;
  dimmerPin2_ = dimmerPin2;
  settings_   = settings;
  
  maxDisableName_ = &LIGHT_MAX_DISABLE_NAME[0];
}

void Light::check( float currentTime ){
  float percent = 0;
  if ( currentTime > getStartPeakTime() && currentTime < getEndPeakTime() )
    percent = 1;
  if ( currentTime > getStartTime() && currentTime < getStartPeakTime() )
    percent = calcRampPercent( currentTime, getStartTime(), getStartPeakTime() );
  if ( currentTime > getEndPeakTime() && currentTime < getEndTime() )
    percent = calcRampPercent( currentTime, getEndTime(), getEndPeakTime() );
 
  if ( getStatus() )
    setLights( percent );
  else
    setLights( 0 );
  
  return;
}

float Light::getBrightness(){
  return brightness_ * 100;
}

void Light::setLights( float percent ){
  brightness_ = percent;
  setDimmer( dimmerPin1_, percent * settings_->get( &LIGHT_MAX_CH1_NAME[0] ) );
  setDimmer( dimmerPin2_, percent * settings_->get( &LIGHT_MAX_CH2_NAME[0] ) );
  return;
}

float Light::calcRampPercent( float currentTime, float loRampTime, float hiRampTime ){
  float percent = ( currentTime - loRampTime ) / ( hiRampTime - loRampTime );
  return percent;
}

void Light::setDimmer( uint8_t dimmerPin, float percent ){
  int value = percent / 100 * 255;
  analogWrite( dimmerPin, value );
  return;
}

float Light::getStartTime(){
  float time = settings_->get(&LIGHT_ON_HOUR_NAME[0]);
  time = time + settings_->get(&LIGHT_ON_MINUTE_NAME[0]) / 60;
  return time;
}

float Light::getStartPeakTime(){
  float time = getStartTime();
  time = time + settings_->get(&LIGHT_RAMP_TIME_NAME[0]) / 60;
  return time;
}

float Light::getEndPeakTime(){
  float time = getStartPeakTime();
  time = time + settings_->get(&LIGHT_PEAK_TIME_NAME[0]);
  return time;
}

float Light::getEndTime(){
  float time = getEndPeakTime();
  time = time + settings_->get(&LIGHT_RAMP_TIME_NAME[0]) / 60;
  return time;
}


