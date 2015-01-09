#ifndef Return_h
#define Return_h

#include <Arduino.h>
#include "Setting.h" 

#define RETURN_PIN 11

class Return{
  public:
    Return( Setting* );
    void enable();
    void disable();
    bool getPumpStatus();
    
  private:
    Setting* settings_;
    bool     pumpStatus_;
    
    void pumpOn();
    void pumpOff();
    void pumpInit();
};

#endif
