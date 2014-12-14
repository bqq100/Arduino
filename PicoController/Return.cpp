#include "Return.h"

Return::Return(){
  if ( RETURN_PIN )
    pinMode( RETURN_PIN, OUTPUT );
  pumpInit();
}

void Return::enable(){
  pumpOn();
}

void Return::disable(){
  pumpOff();
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


