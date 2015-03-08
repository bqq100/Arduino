#include "Waterchange.h"

Waterchange::Waterchange( Setting* settings, Clock* clock, Ato* ato, uint8_t drainPin, uint8_t fillPin ): Equip( settings, clock, drainPin ){
  settings->init( &WC_MAX_DISABLE_NAME[0], &WC_MAX_DISABLE_DESC[0], &WC_MAX_DISABLE_UNIT[0], 10080 ); //10080
  settings->init( &WC_AMOUNT_NAME[0]     , &WC_AMOUNT_DESC[0]     , &WC_AMOUNT_UNIT[0]     , 10  ); //4000
  settings->init( &WC_CAL_NAME[0]        , &WC_CAL_DESC[0]        , &WC_CAL_UNIT[0]        , 1    ); //65
  settings->init( &WC_INCREMENT_NAME[0]  , &WC_INCREMENT_DESC[0]  , &WC_INCREMENT_UNIT[0]  , 1   ); //150
  settings->init( &WC_TIME_NAME[0]       , &WC_TIME_DESC[0]       , &WC_TIME_UNIT[0]       , 0.4    ); //24
  maxDisableName_ = &WC_MAX_DISABLE_NAME[0];
  wcStartTime_ = 0;
  fillPump_ = new Equip( settings, clock, fillPin );
  fillOff();
  ato_ = ato;
}

void Waterchange::check(){
  unsigned long currentEpoch = clock_->getEpoch();
  if ( forceOn_ ){
    wcStartTime_ = currentEpoch;
    enable(); // return to auto state
    atoDisable();
  }
  
  checkDrain( currentEpoch );
  checkFill( currentEpoch );

  if ( wcStartTime_ && !currentlyFilling() && currentEpoch > wcStartTime_ + settings_->get( &WC_TIME_NAME[0] ) * 60 * 60 ){
    wcStartTime_ = 0;
    atoEnable();
  }
}

bool Waterchange::currentlyFilling(){
  return fillPump_->getEquipStatus();
}

bool Waterchange::getWCStatus(){
  if ( wcStartTime_ )
    return true;
  else
    return false;
}

void Waterchange::checkDrain( unsigned long currentEpoch ){
  if ( getStatus() && wcStartTime_ && currentEpoch > wcStartTime_ && currentEpoch < wcStartTime_ + drainTimePerInterval() )
    drainOn();
  else
    drainOff(); 
}

void Waterchange::checkFill( unsigned long currentEpoch ){
  if ( getStatus() && wcStartTime_ && currentEpoch > wcStartTime_ + FILL_DELAY + drainTimePerInterval() ){
    if ( currentEpoch > wcStartTime_ + FILL_DELAY + 3 * drainTimePerInterval() || !ato_->quickHiCheck() ){
      wcStartTime_ = 0;
      fillOff();
    } else if ( ato_->quickLoCheck() ){
      fillOn();
    } else {
      fillOff();
      wcStartTime_ = wcStartTime_ + drainTimeBetweenIntervals();
    }
  }
}

unsigned long Waterchange::drainTimePerInterval(){
  // seconds
  unsigned long drainTimePerInterval = ( settings_->get( &WC_INCREMENT_NAME[0] ) / settings_->get( &WC_CAL_NAME[0] ) ) * 60.0;
  if ( drainTimeBetweenIntervals() <= 3 * drainTimePerInterval + 2 * FILL_DELAY )
    return ( drainTimeBetweenIntervals() - 2 * FILL_DELAY) / 3;
  else
    return drainTimePerInterval;
}

int Waterchange::numberOfDrainIntervals(){
  return settings_->get( &WC_AMOUNT_NAME[0] ) / settings_->get( &WC_INCREMENT_NAME[0] );
}

unsigned long Waterchange::drainTimeBetweenIntervals(){
  // seconds
  return ( settings_->get( &WC_TIME_NAME[0] ) / numberOfDrainIntervals() ) * 60 * 60;
}

void Waterchange::drainOn(){
  equipOn();
}

void Waterchange::drainOff(){
  equipOff();
}

void Waterchange::fillOn(){
  fillPump_->equipOn();
}

void Waterchange::fillOff(){
  fillPump_->equipOff();
}

void Waterchange::atoDisable(){
  if ( ato_->getStatus() )
    ato_->disable();
}

void Waterchange::atoEnable(){
  if ( !ato_->getStatus() )
    ato_->enable();
}

