#include "Ato.h"

// Constructor

Ato::Ato( Setting* settings, Clock* clock, uint8_t atoPumpPin, uint8_t atoHiPin, uint8_t atoLoPin ): Equip( settings, clock, atoPumpPin ){
  
  settings->init( &ATO_ALARM_NAME[0]      , &ATO_ALARM_DESC[0]      , &ATO_ALARM_UNIT[0]      , 60 );
  settings->init( &ATO_MIN_ON_NAME[0]     , &ATO_MIN_ON_DESC[0]     , &ATO_MIN_ON_UNIT[0]     , 10 );
  settings->init( &ATO_MAX_ON_NAME[0]     , &ATO_MAX_ON_DESC[0]     , &ATO_MAX_ON_UNIT[0]     , 30 );
  settings->init( &ATO_DEBOUNCE_NAME[0]   , &ATO_DEBOUNCE_DESC[0]   , &ATO_DEBOUNCE_UNIT[0]   , 3  );
  settings->init( &ATO_PUMP_ALARM_NAME[0] , &ATO_PUMP_ALARM_DESC[0] , &ATO_PUMP_ALARM_UNIT[0] , 60 );
  settings->init( &ATO_MAX_DISABLE_NAME[0], &ATO_MAX_DISABLE_DESC[0], &ATO_MAX_DISABLE_UNIT[0], 60 );

  settings->init( &ATO_LO_INV_NAME[0], &ATO_LO_INV_DESC[0], 0 );
  settings->init( &ATO_HI_INV_NAME[0], &ATO_HI_INV_DESC[0], 1 );
  
  pinMode( atoHiPin,  INPUT_PULLUP );
  pinMode( atoLoPin,  INPUT_PULLUP );
  setLoAlarm   ( false );
  setHiAlarm   ( false );
  setPumpAlarm ( false );
  loWaterTime_  = 0;
  atoHiPin_ = atoHiPin;
  atoLoPin_ = atoLoPin;
  maxDisableName_ = &ATO_MAX_DISABLE_NAME[0];
}

// Public full check including reading inputs, setting alarms and turning on/off pump

void Ato::check(){
  unsigned long currentEpoch = clock_->getEpoch();
  if ( getPumpAlarm() && currentEpoch > pumpAlarmTime_ + (unsigned long)settings_->get( &ATO_PUMP_ALARM_NAME[0] ) * 60 )
    setPumpAlarm(false);
  
  bool waterLo = quickLoCheck();
  bool waterHi = quickHiCheck();

  setHiAlarm( waterHi );
  setLoAlarm( waterLo && loWaterTime_ && currentEpoch > loWaterTime_ + (unsigned long)settings_->get( &ATO_ALARM_NAME[0] ) * 60 );
  setPumpAlarm( getPumpAlarm() || ( getEquipStatus() && currentEpoch > pumpOnTime_ + (unsigned long)settings_->get( &ATO_MAX_ON_NAME[0] ) ) );
  
  if ( !getHiAlarm() && forceOn_ )
    equipOn();
  else if ( !getHiAlarm() && waterLo && !getPumpAlarm() && getStatus() )  // Pump alarm should cover Low Alarm issues
    equipOn();    
  else
    equipOff();  
  
}

// Public functions for quickly checking current status of the float switches
bool Ato::quickLoCheck(){
  unsigned long currentEpoch = clock_->getEpoch();
  bool loFlag = genericCheck( atoLoPin_ );
  
  if ( (bool)settings_->get( &ATO_LO_INV_NAME[0] ) )
    loFlag = !loFlag;
  
  if ( loWaterTime_ == 0  && loFlag == true )
    loWaterTime_ = clock_->getEpoch();
  if ( loFlag == false )
    loWaterTime_ = 0;
    
  if ( loFlag == true && currentEpoch < loWaterTime_ + (unsigned long)settings_->get( &ATO_DEBOUNCE_NAME[0] ) )
    return false;
  else
    return loFlag;
}

bool Ato::quickHiCheck(){
  bool hiFlag = genericCheck( atoHiPin_ );

  if ( (bool)settings_->get( &ATO_HI_INV_NAME[0] ) )
    hiFlag = !hiFlag;
  
  if ( hiWaterTime_ == 0  && hiFlag == true )
    hiWaterTime_ = clock_->getEpoch();
  if ( hiFlag == false )
    hiWaterTime_ = 0;
    
  return hiFlag;
}

// Public Get Flag Functions

bool Ato::getLoAlarm(){
  return loAlarm_;
}

bool Ato::getHiAlarm(){
  return hiAlarm_;
}

bool Ato::getPumpAlarm(){
  return pumpAlarm_;
}


// Private Set Flag functions

void Ato::setLoAlarm( bool flag ){
  loAlarm_ = flag;
}

void Ato::setHiAlarm( bool flag ){
  hiAlarm_ = flag;
}

void Ato::setPumpAlarm( bool flag ){
  if ( flag && !pumpAlarmTime_ )
    pumpAlarmTime_ = clock_->getEpoch();
  if ( !flag )
    pumpAlarmTime_ = 0;
  pumpAlarm_ = flag;
}

// Private Generic Check Function
bool Ato::genericCheck( uint8_t pin ){
  if ( digitalRead( pin ) == LOW )
    return true;
  else
    return false;
}


