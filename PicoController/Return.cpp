#include "Return.h"

Return::Return( Setting* settings ){
  if ( RETURN_PIN )
    pinMode( RETURN_PIN, OUTPUT );
  pumpInit();
  settings->init( &RETURN_MAX_DISABLE_NAME[0], &RETURN_MAX_DISABLE_DESC[0], &RETURN_MAX_DISABLE_UNIT[0], 60 );
  settings_ = settings;
  disableUntil_ = 0;
}


// Public functions for enabling/disabling Return

bool Return::getStatus(){
  if ( disableUntil_ )
    return false;
  else
    return true;
}

bool Return::getPumpStatus(){
  return pumpStatus_;
}

void Return::enable(){
  disableUntil_ = 0;
}

void Return::disable(){
  disableUntil_ = millis() + (unsigned long)settings_->get( &RETURN_MAX_DISABLE_NAME[0] ) * 1000 * 60;
}

void Return::check(){
  if ( !getDisableFlag() )
    pumpOn();
  else
    pumpOff();
}

bool Return::getDisableFlag(){
  bool flag = ( disableUntil_ && millis() < disableUntil_ );
  return flag;
}

// Private Pump Functions

void Return::pumpOn(){
  if ( !getPumpStatus() ){
    pumpStatus_ = true;
    if ( RETURN_PIN )
      digitalWrite( RETURN_PIN, HIGH );
  }
}

void Return::pumpOff(){
  if ( getPumpStatus() ){
    pumpStatus_ = false;
    if ( RETURN_PIN )
      digitalWrite( RETURN_PIN, LOW );
  }
}

void Return::pumpInit(){
  pumpStatus_ = true;
  if ( RETURN_PIN )  
    digitalWrite( RETURN_PIN, HIGH );
}



