#include "Doser.h"

Doser::Doser( Setting* settings, Clock* clock, uint8_t doserPin ): Equip( settings, clock, doserPin ){
  settings->init( &DOSER_MAX_DISABLE_NAME[0], &DOSER_MAX_DISABLE_DESC[0], &DOSER_MAX_DISABLE_UNIT[0], 1440 );
  settings->init( &DOSER_AMOUNT_NAME[0]     , &DOSER_AMOUNT_DESC[0]     , &DOSER_AMOUNT_UNIT[0]     , 0    );
  settings->init( &DOSER_CAL_NAME[0]        , &DOSER_CAL_DESC[0]        , &DOSER_CAL_UNIT[0]        , 35   );
  
  settings->init( &DOSER_NUM_PER_DAY_NAME[0], &DOSER_NUM_PER_DAY_DESC[0], 8 );
  
  maxDisableName_ = &DOSER_MAX_DISABLE_NAME[0];
}

void Doser::check(){

  float currentTime = clock_->getTime();

  int interval = currentTime / timeBetweenDoses(); 

  float startTime = interval * timeBetweenDoses();
  float endTime   = startTime + timePerDose();

  if ( forceOn_ )
    equipOn();
  else if ( currentTime >= startTime && currentTime <= endTime && getStatus() )
    equipOn();
  else
    equipOff();

}

float Doser::timePerDay(){
  return settings_->get( &DOSER_AMOUNT_NAME[0] ) / settings_->get( &DOSER_CAL_NAME[0] ) / 60.0;
}

float Doser::timePerDose(){
  return timePerDay() / settings_->get( &DOSER_NUM_PER_DAY_NAME[0] );
}

float Doser::timeBetweenDoses(){
  return 24.0 / settings_->get( &DOSER_NUM_PER_DAY_NAME[0] );
}
