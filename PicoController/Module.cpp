#include "Module.h"

Module::Module( Setting* settings ){
  settings_ = settings;
  disableUntil_ = 0;
}

// Public functions for enabling/disabling Module
bool Module::getStatus(){
  bool flag = ( disableUntil_ && millis() < disableUntil_ );
  return !flag;
}

void Module::enable(){
  disableUntil_ = 0;
}

void Module::disable(){
  disableUntil_ = millis() + (unsigned long)settings_->get( maxDisableName_ ) * 1000 * 60;
}

