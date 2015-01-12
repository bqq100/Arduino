#ifndef Ato_h
#define Ato_h

#include <Arduino.h>
#include "Utilities.h"
#include "Setting.h"  
  
#define HI_PIN 9
#define LO_PIN 8
#define ATO_PIN 13

static prog_char MIN_ON_NAME[] PROGMEM = "MinOnTime";
static prog_char MIN_ON_DESC[] PROGMEM = "ATO pump minimum on time";
static prog_char MIN_ON_UNIT[] PROGMEM = "secs";

static prog_char MAX_ON_NAME[] PROGMEM = "MaxOnTime";
static prog_char MAX_ON_DESC[] PROGMEM = "ATO pump maximum on time";
static prog_char MAX_ON_UNIT[] PROGMEM = "secs";

static prog_char DEBOUNCE_NAME[] PROGMEM = "DebounceTime";
static prog_char DEBOUNCE_DESC[] PROGMEM = "ATO switch debounce time";
static prog_char DEBOUNCE_UNIT[] PROGMEM = "secs";

static prog_char ALARM_NAME[] PROGMEM = "LowAlarmTime";
static prog_char ALARM_DESC[] PROGMEM = "ATO low water time before raising alarm";
static prog_char ALARM_UNIT[] PROGMEM = "min";

static prog_char PUMP_ALARM_NAME[] PROGMEM = "PumpAlarmTime";
static prog_char PUMP_ALARM_DESC[] PROGMEM = "ATO time between resetting pump alarm";
static prog_char PUMP_ALARM_UNIT[] PROGMEM = "min";

static prog_char MAX_DISABLE_NAME[] PROGMEM = "MaxDisableTime";
static prog_char MAX_DISABLE_DESC[] PROGMEM = "ATO maximum time to be disabled";
static prog_char MAX_DISABLE_UNIT[] PROGMEM = "min";

static prog_char LO_INV_NAME[] PROGMEM = "InvLoSwitch";
static prog_char LO_INV_DESC[] PROGMEM = "ATO Invert Lo Switch [ switch off is error state ]";

static prog_char HI_INV_NAME[] PROGMEM = "InvHiSwitch";
static prog_char HI_INV_DESC[] PROGMEM = "ATO Invert Hi Switch [ switch off is error state ]";

class Ato{
  public:
    Ato( Setting* );

    void check();
    
    void disable();
    void enable();
    
    bool quickLoCheck();
    bool quickHiCheck();
    
    bool getStatus();
    bool getLoAlarm();
    bool getHiAlarm();
    bool getPumpAlarm();
    bool getPumpStatus();
        
  private:
    Setting* settings_;
  
    unsigned long pumpOnTime_;
    unsigned long pumpAlarmTime_;
    unsigned long loWaterTime_;
    unsigned long hiWaterTime_;
    unsigned long lastOutput_;
    unsigned long disableUntil_;
    
    bool disable_;
    bool loAlarm_;
    bool hiAlarm_;
    bool pumpAlarm_;
    bool pumpStatus_;    

    bool genericCheck( uint8_t pin );

    void pumpOn();
    void pumpOff();
    void pumpInit();

    void setPumpAlarm( bool flag );
    void setLoAlarm( bool flag );
    void setHiAlarm( bool flag );
    
    bool getDisableFlag();
};

#endif

