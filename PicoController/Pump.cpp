#include "Pump.h"

Pump::Pump( Setting* settings, uint8_t pumpPin ): Module( settings ){
  pumpPin_ = pumpPin;
  if ( pumpPin_ )
    pinMode( pumpPin_, OUTPUT );
  pumpInit();
}

// Public functions for enabling/disabling Return
bool Pump::getPumpStatus(){
  return pumpStatus_;
}

// Private Pump Functions
void Pump::pumpOn(){
  if ( !getPumpStatus() ){
    pumpStatus_ = true;
    if ( pumpPin_ )
      digitalWrite( pumpPin_, HIGH );
  }
}

void Pump::pumpOff(){
  if ( getPumpStatus() ){
    pumpStatus_ = false;
    if ( pumpPin_ )
      digitalWrite( pumpPin_, LOW );
  }
}

void Pump::pumpInit(){
  pumpStatus_ = true;
  if ( pumpPin_ )  
    digitalWrite( pumpPin_, HIGH );
}



