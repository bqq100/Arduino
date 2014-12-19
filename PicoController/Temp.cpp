#include "Temp.h"

Temp::Temp() : sensor_( TEMP_PIN ){
  hasSensor_ = findSensor();
  sensorReady_ = 0;
}

void Temp::check(){
  if ( !hasSensor_ )
    hasSensor_ = findSensor();
  if ( !sensorReady_ )
    startConversion();
  else{
    lastTemperature_ = temperature_;
    if ( readSensor() ){
      Serial.print( "Temperature = " );
      Serial.print(temperature_,2);
      Serial.print("\n");
    }
  }
}

bool Temp::readSensor(){
  if ( millis() < sensorReady_ )
    return false;
  if ( sensor_.reset() ){
    sensor_.select(addr_);
    sensor_.write(0XBE);
    sensorReady_ = 0;
    readData();
    return true;
  }
  return false;
}

void Temp::readData(){
  int i;
  for (i=0; i<9; i++)
    data_[i] = sensor_.read();
  convertData();
  return;
}  

void Temp::convertData(){
  int loByte = data_[0];
  int hiByte = data_[1];
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

