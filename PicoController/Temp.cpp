#include "Temp.h"

// Constructor

Temp::Temp( Setting* settings ) : sensor_( TEMP_PIN ){
  
  settings->init( &HI_TEMP_NAME[0], &HI_TEMP_DESC[0], &HI_TEMP_UNIT[0], 77.0 );
  settings->init( &LO_TEMP_NAME[0], &LO_TEMP_DESC[0], &LO_TEMP_UNIT[0], 76.0 );
  settings->init( &CAL_FAC_NAME[0], &CAL_FAC_DESC[0], &CAL_FAC_UNIT[0], -0.8 );
  
  hasSensor_   = findSensor();
  sensorReady_ = 0;
  settings_    = settings;
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
      if ( getTemp() > settings_->get( &HI_TEMP_NAME[0] ) )
        heaterOff();
      if ( getTemp() < settings_->get( &LO_TEMP_NAME[0] ) )
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
