#include "Ato.h"
#include <NewPing.h>

Ato::Ato() : newping_( TX_PIN, RX_PIN, INITIAL_MAX_DISTANCE ){
  resetBluetooth();
  setLowFlag  ( false );
  setHighFlag ( false );
  setPumpFlag ( false );
  setErrorFlag( false );
}


void Ato::readSensor( char line[] ){
  int sensor;

  sensor = newping_.convert_mm( newping_.ping_median( NUM_OF_PULSES ) );
  level_ = config_.getHeight() - sensor;

  if ( level_ < config_.getHeight() && level_ > 0 ){

    if ( !lowFlag_ && level_ < config_.getLoLimit() )
      setLowFlag( true );
    else if ( lowFlag_ && level_ > config_.getLoLimit() ) 
      setLowFlag( false );  

    if ( !highFlag_ && level_ > config_.getHiLimit() )
      setHighFlag( true ); 
    else if ( highFlag_ && level_ < config_.getHiLimit() ) 
      setHighFlag( false );

    if ( !pumpFlag_ && level_ < config_.getTarget() )
      setPumpFlag( true );
    else if ( pumpFlag_ && level_ > config_.getTarget() )
      setPumpFlag( false ); 

    if ( errorFlag_ )
      setErrorFlag( false );

    missed_ = 0;
  }else{
    missed_++;

    if (missed_ > 5){
      setErrorFlag( true );
    }
  }
  sprintf( line, "Level: %d Sensor: %d Config: %d:%d:%d:%d", level_, 
           sensor, config_.getHiLimit(), config_.getLoLimit(), 
           config_.getTarget(), config_.getHeight() );
}


int Ato::getLevel(){
  return level_;
}


void Ato::readCommand(){
  String input;
  bool seperator = 0;
  char command;
  if ( Serial.available() > 0) {
    delay( 200 );  //delay to allow buffer to fill
    command = Serial.read();
  }
  while ( Serial.available() ) {
    if ( Serial.available() > 0 ) {
      char c = Serial.read();  //gets one byte from serial buffer
      if ( c == ':' ) {
        seperator = 1;
      }else if ( c == '\n' ) {
        bool status;
        switch ( command ) {
          case 'L':
            status = config_.setLoLimit( input.toInt() );
            break;
          case 'U':
            status = config_.setHiLimit( input.toInt() );
            break;
          case 'T':
            status = config_.setTarget( input.toInt() );
            break;
          case 'H':
            status = config_.setHeight( input.toInt() );
            break;
          case 'R':
            break;
        }
        resetBluetooth();
      }else if ( seperator == 1 ) {
        input += c;
      }
    }
  }
  return;
}


bool Ato::getLowFlag(){
  return lowFlag_;
}


bool Ato::getHighFlag(){
  return highFlag_;
}


bool Ato::getPumpFlag(){
  return pumpFlag_;
}


bool Ato::getErrorFlag(){
  return errorFlag_;
}


void Ato::setLowFlag( bool status ){
  lowFlag_ = status;
}


void Ato::setHighFlag( bool status ){
  highFlag_ = status;
}


void Ato::setPumpFlag( bool status ){
  pumpFlag_ = status;
}


void Ato::setErrorFlag( bool status ){
  errorFlag_ = status;
}


void Ato::resetBluetooth(){
  pinMode(BLUETOOTH_PIN, OUTPUT);
  digitalWrite(BLUETOOTH_PIN, LOW); 
  delay(1000);
  digitalWrite(BLUETOOTH_PIN, HIGH); 
}
