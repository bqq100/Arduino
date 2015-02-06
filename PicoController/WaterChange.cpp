#include "Waterchange.h"

Waterchange::Waterchange( Setting* settings, Clock* clock, Ato* ato, uint8_t drainPin, uint8_t fillPin ): Equip( settings, clock, drainPin ){
  settings->init( &WC_MAX_DISABLE_NAME[0], &WC_MAX_DISABLE_DESC[0], &WC_MAX_DISABLE_UNIT[0], 10080 );
  settings->init( &WC_AMOUNT_NAME[0]     , &WC_AMOUNT_DESC[0]     , &WC_AMOUNT_UNIT[0]     , 4000  );
  settings->init( &WC_CAL_NAME[0]        , &WC_CAL_DESC[0]        , &WC_CAL_UNIT[0]        , 65    );
  settings->init( &WC_INCREMENT_NAME[0]  , &WC_INCREMENT_DESC[0]  , &WC_INCREMENT_UNIT[0]  , 150   );
  settings->init( &WC_TIME_NAME[0]       , &WC_TIME_DESC[0]       , &WC_TIME_UNIT[0]       , 24    );
  fillPump_ = new Equip( settings, clock, fillPin );
  ato_ = ato;
}

void Waterchange::check(){
}
/*
float Doser::timePerDay(){
  return settings_->get( &DOSER_AMOUNT_NAME[0] ) / settings_->get( &DOSER_CAL_NAME[0] );
}

float Doser::timePerDose(){
  return timePerDay() / settings_->get( &DOSER_NUM_PER_DAY_NAME[0] );
}

float Doser::timeBetweenDoses(){
  return 24 * 60 / settings_->get( &DOSER_NUM_PER_DAY_NAME[0] );
}
*/
