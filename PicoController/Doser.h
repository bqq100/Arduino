#ifndef Doser_h
#define Doser_h

#include "Equip.h"

static prog_char DOSER_MAX_DISABLE_NAME[] PROGMEM = "DoserMaxDisableTime";
static prog_char DOSER_MAX_DISABLE_DESC[] PROGMEM = "Doser maximum time to be disabled";
static prog_char DOSER_MAX_DISABLE_UNIT[] PROGMEM = "min";

static prog_char DOSER_NUM_PER_WEEK_NAME[] PROGMEM = "DosesPerWeek";
static prog_char DOSER_NUM_PER_WEEK_DESC[] PROGMEM = "Doser number of doses per week";

static prog_char DOSER_AMOUNT_NAME[] PROGMEM = "DoserAmount";
static prog_char DOSER_AMOUNT_DESC[] PROGMEM = "Doser quantity";
static prog_char DOSER_AMOUNT_UNIT[] PROGMEM = "mL";

static prog_char DOSER_CAL_NAME[] PROGMEM = "DoserCalibration";
static prog_char DOSER_CAL_DESC[] PROGMEM = "Doser calibration";
static prog_char DOSER_CAL_UNIT[] PROGMEM = "ml/min";

class Doser: public Equip{
  public:
    Doser( Setting*, uint8_t );
    void check( float );
    
  private:
};

#endif


