#include <Arduino.h>
#include "Utilities.h"

const uint8_t HI_PIN = 9;
const bool    HI_INV = true;

const uint8_t LO_PIN = 8;
const bool    LO_INV = false;

const uint8_t ATO_PIN = 13;

// Seconds
const uint8_t MIN_ON_TIME = 10;
const uint8_t MAX_ON_TIME = 30;  // was 60

// Minutes
const uint8_t ALARM_TIME = 60; // was 60
const uint8_t PUMP_ALARM_RESET_TIME = 60; // was 60
const uint8_t MAX_DISABLE_TIME = 60;  // was 60

class Ato{
  public:
    Ato();

    void fullCheck();
    
    void disable();
    void enable();
    
    bool quickLoCheck();
    bool quickHiCheck();
    
    bool getLoAlarm();
    bool getHiAlarm();
    bool getPumpAlarm();
    bool getPumpStatus();
        
  private:
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

