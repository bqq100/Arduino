#include "Ato.h"

// Constructor

Ato::Ato(){
  pinMode( HI_PIN,  INPUT_PULLUP );
  pinMode( LO_PIN,  INPUT_PULLUP );
  pinMode( ATO_PIN, OUTPUT );
  pumpInit();
  setLoAlarm   ( false );
  setHiAlarm   ( false );
  setPumpAlarm ( false );
  loWaterTime_ = 0;
  disableUntil_ = 0;
  lastOutput_ = 0;
}

// Public full check including reading inputs, setting alarms and turning on/off pump

void Ato::check(){

  if ( getPumpAlarm() && millis() > pumpAlarmTime_ + (unsigned long)PUMP_ALARM_RESET_TIME * 1000 * 60 )
    setPumpAlarm(false);
  
  bool waterLo = quickLoCheck();
  bool waterHi = quickHiCheck();

  setHiAlarm( waterHi );
  setLoAlarm( waterLo && loWaterTime_ && millis() > loWaterTime_ + (unsigned long)ALARM_TIME * 1000 * 60 );
  setPumpAlarm( getPumpAlarm() || ( getPumpStatus() && millis() > pumpOnTime_ + (unsigned long)MAX_ON_TIME * 1000 ) );
  
  if ( waterLo && !getHiAlarm() && !getPumpAlarm() && !getDisableFlag() )  // Pump alarm should cover Low Alarm issues
    pumpOn();    
  else
    pumpOff();  
  
  if ( millis() > lastOutput_ + 1000 ){
    lastOutput_ = millis();
  }
}

// Public functions for enabling/disabling ATO for water changes etc

void Ato::enable(){
  disableUntil_ = 0;
}

void Ato::disable(){
  disableUntil_ = millis() + (unsigned long)MAX_DISABLE_TIME * 1000 * 60;
}

// Public functions for quickly checking current status of the float switches

bool Ato::quickLoCheck(){
  bool loFlag = genericCheck( LO_PIN );
  
  if ( LO_INV )
    loFlag = !loFlag;
  
  if ( loWaterTime_ == 0  && loFlag == true )
    loWaterTime_ = millis();
  if ( loFlag == false )
    loWaterTime_ = 0;
    
  if ( loFlag == true && millis() < loWaterTime_ + (unsigned long)DEBOUNCE_TIME * 1000 )
    return false;
  else
    return loFlag;
}

bool Ato::quickHiCheck(){
  bool hiFlag = genericCheck( HI_PIN );

  if ( HI_INV )
    hiFlag = !hiFlag;
  
  if ( hiWaterTime_ == 0  && hiFlag == true )
    hiWaterTime_ = millis();
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

bool Ato::getPumpStatus(){
  return pumpStatus_;
}





// Private Get Flag functions

bool Ato::getDisableFlag(){
  bool flag = ( disableUntil_ && millis() < disableUntil_ );
  return flag;
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
    pumpAlarmTime_ = millis();
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

// Private Pump Functions

void Ato::pumpOn(){
  if ( !getPumpStatus() ){
    pumpStatus_ = true;
    pumpOnTime_ = millis();
    if ( ATO_PIN )
      digitalWrite( ATO_PIN, HIGH );
  }
}

void Ato::pumpOff(){
  // Hi alarm and manually disabled override minimum pump on time
  if ( getHiAlarm() || getDisableFlag() || ( getPumpStatus() && millis() > pumpOnTime_ + (unsigned long)MIN_ON_TIME * 1000 ) ){
    pumpStatus_ = false;
    pumpOnTime_ = 0;
    if ( ATO_PIN )
      digitalWrite( ATO_PIN, LOW );
  }
}

void Ato::pumpInit(){
  pumpStatus_ = false;
  pumpOnTime_ = 0;
  if ( ATO_PIN )
    digitalWrite( ATO_PIN, LOW );
}

