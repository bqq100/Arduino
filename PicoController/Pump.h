#ifndef Pump_h
#define Pump_h

#include "Module.h"

class Pump: public Module{
  public:
    Pump( Setting*, uint8_t );
    bool getPumpStatus();
    
  protected:
    bool pumpStatus_;
    int  pumpPin_;

    void pumpOn();
    void pumpOff();
    void pumpInit();
};

#endif
