#include "Equip.h"

Equip::Equip( Setting* settings, Clock* clock, uint8_t equipPin ): Module( settings, clock ){
  equipPin_ = equipPin;
  if ( equipPin_ )
    pinMode( equipPin_, OUTPUT );
  equipInit();
}

// Public functions for enabling/disabling Return
bool Equip::getEquipStatus(){
  return equipStatus_;
}

// Private Pump Functions
void Equip::equipOn(){
  if ( !getEquipStatus() ){
    equipStatus_ = true;
    equipOnTime_ = clock_->getEpoch();
    if ( equipPin_ )
      digitalWrite( equipPin_, HIGH );
  }
}

void Equip::equipOff(){
  if ( getEquipStatus() ){
    equipStatus_ = false;
    if ( equipPin_ )
      digitalWrite( equipPin_, LOW );
  }
}

void Equip::equipInit(){
  equipStatus_ = true;
  equipOnTime_ = 0;
  if ( equipPin_ )  
    digitalWrite( equipPin_, HIGH );
}

