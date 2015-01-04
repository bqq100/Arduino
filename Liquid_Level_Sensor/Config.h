#include <Arduino.h>
#include <EEPROM.h>

const int HI_LIM_ADDR = 0;
const int LO_LIM_ADDR = 2;
const int TARGET_ADDR = 4;
const int HEIGHT_ADDR = 6;

struct EepromInt{
   byte byte1;
   byte byte2;
};

class Config{
  public:
    Config();
    void readConfig();
    int  getHiLimit();
    int  getLoLimit();
    int  getTarget();
    int  getHeight();
    bool setHiLimit( int value );
    bool setLoLimit( int value );
    bool setTarget ( int value );
    bool setHeight ( int value );
  private:
    int hiLimit_;
    int loLimit_;
    int target_;
    int height_;
    EepromInt convertIntToBytes( int integer );
};

