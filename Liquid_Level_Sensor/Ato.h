#include <Arduino.h>
#include "NewPing.h"
#include "Config.h"

const uint8_t TX_PIN = 12;
const uint8_t RX_PIN = 13;
const int     INITIAL_MAX_DISTANCE = 500;
const int     NUM_OF_PULSES = 20;

const uint8_t BLUETOOTH_PIN = 8;

const uint8_t LOW_FLAG_PIN = 7;
const uint8_t HIGH_FLAG_PIN = 8;
const uint8_t PUMP_FLAG_PIN = 9;
const uint8_t ERROR_FLAG_PIN = 10;

class Ato{
  public :
    Ato();
    void readSensor( char line[] );
    void readCommand();
    int  getLevel();
    bool getLowFlag();
    bool getHighFlag();
    bool getPumpFlag();
    bool getErrorFlag();
  private : 
    Config config_;
    NewPing newping_;
    int missed_;
    bool lowFlag_;
    bool highFlag_;
    bool pumpFlag_;
    bool errorFlag_;
    int level_;
//    void setPumpOn();
//    void setPumpOff();
    void setLowFlag  ( bool status);
    void setHighFlag ( bool status);
    void setPumpFlag ( bool status);
    void setErrorFlag( bool status);
    void resetBluetooth();
};
