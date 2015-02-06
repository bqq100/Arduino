#include "Temp.h"

// Constructor

Temp::Temp( Setting* settings, Clock* clock, uint8_t heaterPin, uint8_t tempPin ) : Equip( settings, clock, heaterPin), sensor_( tempPin ){
  settings->init( &TEMP_MAX_DISABLE_NAME[0], &TEMP_MAX_DISABLE_DESC[0], &TEMP_MAX_DISABLE_UNIT[0], 60.0 );
  settings->init( &HI_TEMP_NAME[0], &HI_TEMP_DESC[0], &HI_TEMP_UNIT[0], 77.0 );
  settings->init( &LO_TEMP_NAME[0], &LO_TEMP_DESC[0], &LO_TEMP_UNIT[0], 76.0 );
  settings->init( &CAL_FAC_NAME[0], &CAL_FAC_DESC[0], &CAL_FAC_UNIT[0], -0.8 );
  tempPin_     = tempPin;
  hasSensor_   = findSensor();
  sensorReady_ = 0;
  
  maxDisableName_ = &TEMP_MAX_DISABLE_NAME[0];
}

// Public full check function including turning on/off heater

void Temp::check(){
  if ( forceOn_ )
    equipOn();
  else{
    if ( !hasSensor_ )
      hasSensor_ = findSensor();
    if ( !sensorReady_ )
      startConversion();
    else{
      if ( readSensor() ){
        if ( getTemp() > settings_->get( &HI_TEMP_NAME[0] ) )
          equipOff();
        if ( getTemp() < settings_->get( &LO_TEMP_NAME[0] ) )
          equipOn();
      }
    }
  }
}

// Public functions to access current status and temp

float Temp::getTemp(){
  return temperature_;
}

// Private functions to read temp and convert to usable reading

bool Temp::readSensor(){
  byte data[2];
  if ( millis() < sensorReady_ )
    return false;
  if ( sensor_.reset() ){
    sensor_.select(addr_);
    sensor_.write (0XBE);
    data[0] = sensor_.read();
    data[1] = sensor_.read();
    convertData(data[0], data[1]);
    sensorReady_ = 0;
    return true;
  }
  return false;
}

void Temp::convertData(int loByte, int hiByte){
  int tempReading = ( hiByte << 8 ) + loByte;

  int signBit = tempReading & 0x8000;
  if (signBit)
    tempReading = (tempReading ^ 0xffff) + 1;

  float celcius = ( 6.0 * (float)tempReading ) + (float)tempReading / 4.0;
  if (signBit)
    celcius = celcius * -1.0;
    
  temperature_ = celcius / 100.0 * 1.8 + 32.0;
  temperature_ = temperature_ + settings_->get( &CAL_FAC_NAME[0] );

  return;  
}

void Temp::startConversion(){
  sensor_.reset();
  sensor_.select(addr_);
  sensor_.write(0x44,1);
  sensorReady_ = millis() + READ_DELAY;
}

// Private function to find the temp sensor

bool Temp::findSensor(){
  sensor_.reset_search();
  while ( sensor_.search(addr_) ){
    if ( OneWire::crc8( addr_, 7 ) != addr_[7] )
      return false;
    if ( addr_[0] == 0x28 )
      return true;
    else
      continue;
  }
  return false;
}
