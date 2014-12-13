#include <Arduino.h>

const uint8_t TX_PIN = 12;
const uint8_t RX_PIN = 13;
const uint8_t ATO_PIN = 14;

// Seconds
const uint8_t MIN_ON_TIME = 10;
const uint8_t MAX_ON_TIME = 60;

// Minutes
const uint8_t WARN_TIME = 30;
const uint8_t ALARM_TIME = 60;

class Ato{
  public:
    Ato();
    void check();
    bool getLoAlarm();
    bool getHiAlarm();
    bool getLoWarn();
  private:
    void pumpOn();
    void pumpOff();
    unsigned long pumpOnTime;
    unsigned long alarmOnTime;
    bool loAlarm;
    bool hiAlarm;
    bool loWarn;
    bool pumpStatus;    
}
