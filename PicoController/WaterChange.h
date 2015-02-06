#ifndef Waterchange_h
#define Waterchange_h

#include "Equip.h"
#include "Ato.h"

static prog_char WC_MAX_DISABLE_NAME[] PROGMEM = "WCMaxDisableTime";
static prog_char WC_MAX_DISABLE_DESC[] PROGMEM = "Water change maximum time to be disabled";
static prog_char WC_MAX_DISABLE_UNIT[] PROGMEM = "min";

static prog_char WC_INCREMENT_NAME[] PROGMEM = "WCIncrementSize";
static prog_char WC_INCREMENT_DESC[] PROGMEM = "Water change size of each incremental change";
static prog_char WC_INCREMENT_UNIT[] PROGMEM = "mL";

static prog_char WC_AMOUNT_NAME[] PROGMEM = "WCAmount";
static prog_char WC_AMOUNT_DESC[] PROGMEM = "Water change quantity per day";
static prog_char WC_AMOUNT_UNIT[] PROGMEM = "mL";

static prog_char WC_CAL_NAME[] PROGMEM = "WCCalibration";
static prog_char WC_CAL_DESC[] PROGMEM = "Water change pump calibration";
static prog_char WC_CAL_UNIT[] PROGMEM = "ml/min";

static prog_char WC_TIME_NAME[] PROGMEM = "WCTime";
static prog_char WC_TIME_DESC[] PROGMEM = "Water change time to complete";
static prog_char WC_TIME_UNIT[] PROGMEM = "hrs";

class Waterchange: public Equip{
  public:
    Waterchange( Setting*, Clock*, Ato*, uint8_t, uint8_t );
    void check ();
    
  private:
    Ato*   ato_;
    Equip* fillPump_;
  
};

#endif


