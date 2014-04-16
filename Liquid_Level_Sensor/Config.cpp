#include "Config.h"
#include <EEPROM.h>

Config::Config(){
  readConfig();
}


void Config::readConfig(){
  loLimit_ = EEPROM.read( LO_LIM_ADDR + 1 ) * 256 + EEPROM.read( LO_LIM_ADDR );
  hiLimit_ = EEPROM.read( HI_LIM_ADDR + 1 ) * 256 + EEPROM.read( HI_LIM_ADDR );
  target_  = EEPROM.read( TARGET_ADDR + 1 ) * 256 + EEPROM.read( TARGET_ADDR );
  height_  = EEPROM.read( HEIGHT_ADDR + 1 ) * 256 + EEPROM.read( HEIGHT_ADDR );

  return;
}


int Config::getHiLimit(){
  return hiLimit_;
}


int Config::getLoLimit(){
  return loLimit_;
}


int Config::getTarget(){
  return target_;
}


int Config::getHeight(){
  return height_;
}


bool Config::setHiLimit( int value ){
  int readValue;

  EepromInt eepromInt = convertIntToBytes( value );

  EEPROM.write( HI_LIM_ADDR    , eepromInt.byte1 );
  EEPROM.write( HI_LIM_ADDR + 1, eepromInt.byte2 );
   
  readValue = EEPROM.read( HI_LIM_ADDR + 1 ) * 256 + EEPROM.read( HI_LIM_ADDR ); 

  if ( readValue == value ){
    hiLimit_ = value;
    return true;
  }else{
    return false;
  }
}


bool Config::setLoLimit( int value ){
  int readValue;

  EepromInt eepromInt = convertIntToBytes( value );

  EEPROM.write( LO_LIM_ADDR    , eepromInt.byte1 );
  EEPROM.write( LO_LIM_ADDR + 1, eepromInt.byte2 );

  readValue = EEPROM.read( LO_LIM_ADDR + 1 ) * 256 + EEPROM.read( LO_LIM_ADDR );

  if (readValue == value){
    loLimit_ = value;
    return true;
  }else{
    return false;
  }
}


bool Config::setTarget( int value ){
  int readValue;

  EepromInt eepromInt = convertIntToBytes( value );

  EEPROM.write( TARGET_ADDR    , eepromInt.byte1 );
  EEPROM.write( TARGET_ADDR + 1, eepromInt.byte2 );

  readValue = EEPROM.read( TARGET_ADDR + 1 ) * 256 + EEPROM.read( TARGET_ADDR );

  if (readValue == value){
    target_ = value;
    return true;
  }else{
    return false;
  }
}


bool Config::setHeight( int value ){
  int readValue;

  EepromInt eepromInt = convertIntToBytes( value );

  EEPROM.write( HEIGHT_ADDR    , eepromInt.byte1 );
  EEPROM.write( HEIGHT_ADDR + 1, eepromInt.byte2 );

  readValue = EEPROM.read( HEIGHT_ADDR + 1 ) * 256 + EEPROM.read( HEIGHT_ADDR );

  if (readValue == value){
    height_ = value;
    return true;
  }else{
    return false;
  }
}


EepromInt Config::convertIntToBytes( int integer ){
  EepromInt eepromInt;
  eepromInt.byte1 = (byte) integer;
  eepromInt.byte2 = (byte) (integer >> 8);
  return eepromInt;
}
