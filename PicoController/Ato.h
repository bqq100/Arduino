#ifndef Ato_h
#define Ato_h

#include "Equip.h"

static prog_char ATO_MIN_ON_NAME[] PROGMEM = "MinOnTime";
static prog_char ATO_MIN_ON_DESC[] PROGMEM = "ATO pump minimum on time";
static prog_char ATO_MIN_ON_UNIT[] PROGMEM = "secs";

static prog_char ATO_MAX_ON_NAME[] PROGMEM = "MaxOnTime";
static prog_char ATO_MAX_ON_DESC[] PROGMEM = "ATO pump maximum on time";
static prog_char ATO_MAX_ON_UNIT[] PROGMEM = "secs";

static prog_char ATO_DEBOUNCE_NAME[] PROGMEM = "DebounceTime";
static prog_char ATO_DEBOUNCE_DESC[] PROGMEM = "ATO switch debounce time";
static prog_char ATO_DEBOUNCE_UNIT[] PROGMEM = "secs";

static prog_char ATO_ALARM_NAME[] PROGMEM = "LowAlarmTime";
static prog_char ATO_ALARM_DESC[] PROGMEM = "ATO low water time before raising alarm";
static prog_char ATO_ALARM_UNIT[] PROGMEM = "min";

static prog_char ATO_PUMP_ALARM_NAME[] PROGMEM = "PumpAlarmTime";
static prog_char ATO_PUMP_ALARM_DESC[] PROGMEM = "ATO time between resetting pump alarm";
static prog_char ATO_PUMP_ALARM_UNIT[] PROGMEM = "min";

static prog_char ATO_MAX_DISABLE_NAME[] PROGMEM = "AtoMaxDisableTime";
static prog_char ATO_MAX_DISABLE_DESC[] PROGMEM = "ATO maximum time to be disabled";
static prog_char ATO_MAX_DISABLE_UNIT[] PROGMEM = "min";

static prog_char ATO_LO_INV_NAME[] PROGMEM = "InvLoSwitch";
static prog_char ATO_LO_INV_DESC[] PROGMEM = "ATO Invert Lo Switch [ switch off is error state ]";

static prog_char ATO_HI_INV_NAME[] PROGMEM = "InvHiSwitch";
static prog_char ATO_HI_INV_DESC[] PROGMEM = "ATO Invert Hi Switch [ switch off is error state ]";

class Ato: public Equip{
  public:
    Ato( Setting*, Clock*, uint8_t, uint8_t, uint8_t );

    void check();
    
    bool quickLoCheck();
    bool quickHiCheck();
 
    bool getLoAlarm();
    bool getHiAlarm();
    bool getPumpAlarm();
        
  private:
    unsigned long pumpAlarmTime_;
    unsigned long loWaterTime_;
    unsigned long hiWaterTime_;
    
    uint8_t atoLoPin_;
    uint8_t atoHiPin_;
    
    bool loAlarm_;
    bool hiAlarm_;
    bool pumpAlarm_;
 
    bool genericCheck( uint8_t pin );

    void setPumpAlarm( bool flag );
    void setLoAlarm  ( bool flag );
    void setHiAlarm  ( bool flag );
};

#endif

