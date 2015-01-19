#ifndef Light_h
#define Light_h

#include "Module.h"

static prog_char LIGHT_MAX_DISABLE_NAME[] PROGMEM = "LightMaxDisableTime";
static prog_char LIGHT_MAX_DISABLE_DESC[] PROGMEM = "Lighting maximum time to be disabled";
static prog_char LIGHT_MAX_DISABLE_UNIT[] PROGMEM = "min";

static prog_char LIGHT_ON_HOUR_NAME[] PROGMEM = "LightOnHour";
static prog_char LIGHT_ON_HOUR_DESC[] PROGMEM = "Lighting hour to turn on";

static prog_char LIGHT_ON_MINUTE_NAME[] PROGMEM = "LightOnMinute";
static prog_char LIGHT_ON_MINUTE_DESC[] PROGMEM = "Lighting minute to turn on";

static prog_char LIGHT_RAMP_TIME_NAME[] PROGMEM = "LightRampTime";
static prog_char LIGHT_RAMP_TIME_DESC[] PROGMEM = "Lighting time to ramp from 0 to 100%";
static prog_char LIGHT_RAMP_TIME_UNIT[] PROGMEM = "min";

static prog_char LIGHT_PEAK_TIME_NAME[] PROGMEM = "LightPeakTime";
static prog_char LIGHT_PEAK_TIME_DESC[] PROGMEM = "Lighting time at 100%";
static prog_char LIGHT_PEAK_TIME_UNIT[] PROGMEM = "hours";

static prog_char LIGHT_MAX_CH1_NAME[] PROGMEM = "LightCh1MaxPercent";
static prog_char LIGHT_MAX_CH1_DESC[] PROGMEM = "Lighting CH1 peak intensity";
static prog_char LIGHT_MAX_CH1_UNIT[] PROGMEM = "%";

static prog_char LIGHT_MAX_CH2_NAME[] PROGMEM = "LightCh2MaxPercent";
static prog_char LIGHT_MAX_CH2_DESC[] PROGMEM = "Lighting CH2 peak intensity";
static prog_char LIGHT_MAX_CH2_UNIT[] PROGMEM = "%";

class Light: public Module{
  public:
    Light( Setting*, uint8_t, uint8_t );
    void check( float );
    
  private:
};

#endif

