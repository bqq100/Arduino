#ifndef Temp_h
#define Temp_h

#include <Arduino.h>
#include <OneWire.h>
#include "Setting.h" 
#include "Utilities.h"
    
#define TEMP_PIN   10
#define HEATER_PIN 12
#define READ_DELAY 750;

static prog_char HI_TEMP_NAME[] PROGMEM = "HiTemp";
static prog_char HI_TEMP_DESC[] PROGMEM = "Temp to turn off heater";
static prog_char HI_TEMP_UNIT[] PROGMEM = "F";

static prog_char LO_TEMP_NAME[] PROGMEM = "LoTemp";
static prog_char LO_TEMP_DESC[] PROGMEM = "Temp to turn on  heater";
static prog_char LO_TEMP_UNIT[] PROGMEM = "F";

static prog_char CAL_FAC_NAME[] PROGMEM = "CalFactor";
static prog_char CAL_FAC_DESC[] PROGMEM = "Temp sensor calibration";
static prog_char CAL_FAC_UNIT[] PROGMEM = "F";

class Temp{
  public :
    Temp( Setting* );
    void  check();
    bool  getHeaterStatus();
    float getTemp();
    
  private :
    OneWire  sensor_;
    Setting* settings_;
    byte     addr_[8];
    bool     hasSensor_;
    bool     heaterStatus_;
    float    temperature_;
    unsigned long sensorReady_;
    
    bool findSensor();
    bool readSensor();
    void startConversion();
    void convertData(int, int);
    
    void heaterOn();
    void heaterOff();
    void heaterInit();
};
 
#endif
