#include "Module.h"

Module::Module( Setting* settings, Clock* clock ){
  settings_ = settings;
  clock_ = clock;
  disableUntil_ = 0;
  forceOn_ = false;
}

// Public functions for enabling/disabling Module
bool Module::getStatus(){
  unsigned long currentEpoch = clock_->getEpoch();
  bool flag = ( disableUntil_ && currentEpoch < disableUntil_ );
  return !flag;
}

void Module::enable(){
  disableUntil_ = 0;
  forceOn_ = false;
}

void Module::disable(){
  disableUntil_ = clock_->getEpoch();
  disableUntil_ = disableUntil_ + (unsigned long)settings_->get( maxDisableName_ ) * 60;
  forceOn_ = false;
}

void Module::forceOn(){
  disableUntil_ = 0;
  forceOn_ = true;
}

