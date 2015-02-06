#ifndef Module_h
#define Module_h

#include <Arduino.h>
#include "Setting.h"
#include "Clock.h"

class Module{
  public:
    Module( Setting*, Clock* );
	
    void forceOn();
    void enable();
    void disable();
    bool getStatus();
	
  protected:
    unsigned long disableUntil_;
    Setting* settings_;
    Clock* clock_;
    prog_char* maxDisableName_;
    bool forceOn_;

};

#endif
