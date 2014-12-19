#include <Arduino.h>
#include <OneWire.h>
#include "Utilities.h"
    
const uint8_t TEMP_PIN = 10;
const uint8_t HEATER_PIN = 12;
const unsigned long READ_DELAY = 750;

const float CAL_FACTOR = -0.8;

class Temp{
  public :
    Temp();
    void check();
  private :
    bool hasSensor_;
    float temperature_;
    float lastTemperature_;
    unsigned long sensorReady_;
    OneWire sensor_;
    byte addr_[8];
    byte data_[12];
    bool findSensor();
    bool readSensor();
    void startConversion();
    void readData();
    void convertData();
};
 

