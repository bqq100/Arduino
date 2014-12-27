#ifndef Temp_h
#define Temp_h

#include <Arduino.h>
#include <OneWire.h>
#include "Utilities.h"
    
const uint8_t TEMP_PIN = 10;
const uint8_t HEATER_PIN = 12;

const float HI_TEMP = 77.0;
const float LO_TEMP = 76.0;

const float CAL_FACTOR = -0.8;

const unsigned long READ_DELAY = 750;

class Temp{
  public :
    Temp();
    void  check();
    bool  getHeaterStatus();
    float getTemp();
    
  private :
    OneWire sensor_;
    byte    addr_[8];
    bool    hasSensor_;
    bool    heaterStatus_;
    float   temperature_;
    unsigned long sensorReady_;
    
    bool findSensor();
    bool readSensor();
    void startConversion();
    void convertData(int loByte, int hiByte);
    
    void heaterOn();
    void heaterOff();
    void heaterInit();
};
 
#endif
