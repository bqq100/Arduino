#ifndef Return_h
#define Return_h

#include <Arduino.h>
#include "Setting.h" 

#define RETURN_PIN 11

static prog_char RETURN_MAX_DISABLE_NAME[] PROGMEM = "MaxReturnDisableTime";
static prog_char RETURN_MAX_DISABLE_DESC[] PROGMEM = "Return Pump maximum time to be disabled";
static prog_char RETURN_MAX_DISABLE_UNIT[] PROGMEM = "min";

class Return{
  public:
    Return( Setting* );
    void enable();
    void disable();
    void check();
    bool getPumpStatus();
    bool getStatus();
    
  private:
    unsigned long disableUntil_;
    Setting* settings_;
    bool     pumpStatus_;

    bool getDisableFlag();
    
    void pumpOn();
    void pumpOff();
    void pumpInit();
};

#endif
