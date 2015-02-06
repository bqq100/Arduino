#ifndef Return_h
#define Return_h

#include "Equip.h"

static prog_char RETURN_MAX_DISABLE_NAME[] PROGMEM = "MaxReturnDisTime";
static prog_char RETURN_MAX_DISABLE_DESC[] PROGMEM = "Return Pump maximum time to be disabled";
static prog_char RETURN_MAX_DISABLE_UNIT[] PROGMEM = "min";

class Return: public Equip{
  public:
    Return( Setting*, Clock*, uint8_t );
    void check();
    
  private:
};

#endif
