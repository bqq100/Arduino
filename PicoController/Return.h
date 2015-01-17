#ifndef Return_h
#define Return_h

#include "Pump.h"

static prog_char RETURN_MAX_DISABLE_NAME[] PROGMEM = "MaxReturnDisableTime";
static prog_char RETURN_MAX_DISABLE_DESC[] PROGMEM = "Return Pump maximum time to be disabled";
static prog_char RETURN_MAX_DISABLE_UNIT[] PROGMEM = "min";

class Return: public Pump{
  public:
    Return( Setting*, uint8_t );
    void check();
    
  private:
};

#endif
