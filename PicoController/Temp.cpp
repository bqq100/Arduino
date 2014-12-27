#include "Temp.h"

// Constructor

Temp::Temp() : sensor_( TEMP_PIN ){
  hasSensor_ = findSensor();
  sensorReady_ = 0;
  heaterInit();
}

// Public full check function including turning on/off heater

void Temp::check(){
  if ( !hasSensor_ )
    hasSensor_ = findSensor();
  if ( !sensorReady_ )
    startConversion();
  else
    if ( readSensor() ){
      if ( getTemp() > HI_TEMP )
        heaterOff();
      if ( getTemp() < LO_TEMP )
        heaterOn();
    }
}

// Public functions to access current status and temp

bool Temp::getHeaterStatus(){
  return heaterStatus_;
}

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
    sensor_.write(0XBE);
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

  float celcius = ( 6 * tempReading ) + tempReading / 4;
  if (signBit)
    celcius = celcius * -1;
    
  temperature_ = celcius / 100 * 1.8 + 32;
  temperature_ = temperature_ + CAL_FACTOR;

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

// Private Heater Functions

void Temp::heaterOn(){
  if ( !getHeaterStatus() ){
    heaterStatus_ = true;
    if ( HEATER_PIN )
      digitalWrite( HEATER_PIN, HIGH );
  }
}

void Temp::heaterOff(){
  if ( getHeaterStatus() ){
    heaterStatus_ = false;
    if ( HEATER_PIN )
      digitalWrite( HEATER_PIN, LOW );
  }
}

void Temp::heaterInit(){
  heaterStatus_ = false;
  if ( HEATER_PIN )
    digitalWrite( HEATER_PIN, LOW );
}
