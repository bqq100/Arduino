#ifndef Temp_h
#define Temp_h

#include <OneWire.h>
#include "Equip.h"

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

class Temp: public Equip{
  public :
    Temp( Setting*, uint8_t, uint8_t );
    void  check( float );
    float getTemp();
    
  private :
    uint8_t  tempPin_;
    OneWire  sensor_;
    byte     addr_[8];
    bool     hasSensor_;
    float    temperature_;
    unsigned long sensorReady_;
    
    bool findSensor();
    bool readSensor();
    void startConversion();
    void convertData(int, int);
};
 
#endif
