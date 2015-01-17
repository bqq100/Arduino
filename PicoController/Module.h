#ifndef Module_h
#define Module_h

#include <Arduino.h>
#include "Setting.h" 

class Module{
  public:
    Module( Setting* );
	
    void enable();
    void disable();
    bool getStatus();
	
  protected:
    unsigned long disableUntil_;
    Setting* settings_;
    prog_char* maxDisableName_;

    bool getDisableFlag();
};

#endif
