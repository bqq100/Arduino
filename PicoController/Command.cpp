#include "Command.h"

Command::Command(Ato* ato, Temp* temperature, Return* returnPump){
  Serial.begin( 9600 );
  Serial.println( "Controller Starting Up..." );
  ato_ = ato;
  temperature_ = temperature;
  returnPump_ = returnPump;
  nextStatus_ = 0;
  autoStatus_ = 0;
  autoStatusFrequency_ = 5;
  maxAutoStatusHours_ = 24 * 7;
  input_ = "";
}

void Command::check(){
  char c;
  
  if ( autoStatus_ && nextStatus_ && millis() > nextStatus_ )
    output(status("status",""));
  if ( autoStatus_ && millis() > autoStatus_ )
    autoStatus_ = 0;
  
  while ( Serial.available() > 0 ){
    c = Serial.read();
    Serial.print(c);
    if ( c == '\n' || c == '\r' ){
      if ( input_.length() > 0 ){
        input_.trim();
        processCommand( input_ );
        input_ = "";
      }
    }
    else
      input_ = input_ + c;      
  }
}

void Command::processCommand( String input ){
  String outputLine, command, option;
  
  if ( input.indexOf(':') ){
    command = input.substring( 0, input.indexOf(':') );
    option  = input.substring( input.indexOf(':') + 1 );
  }else{
    command = input;
    option = "";
  }
  
  if ( command == "status" )
    outputLine = status( command, option );
    
  output( outputLine );
}

void Command::output(String outputLine){
  Serial.print(outputLine);
}

String Command::status( String command, String option ){
  if ( option == "stop" ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( option == "start" ){
    autoStatus_ = millis() + (unsigned long)maxAutoStatusHours_ * 1000 * 60 * 60;
    nextStatus_ = millis() + (unsigned long)autoStatusFrequency_ * 1000;
  }
  
  if ( autoStatus_ )
    nextStatus_ = millis() + (unsigned long)autoStatusFrequency_ * 1000;
    
  String line;
  line = line + "Low Switch : " + boolToString(ato_->quickLoCheck()) + "\n\r";
  line = line + "High Switch: " + boolToString(ato_->quickHiCheck()) + "\n\r";
  line = line + "Pump Status: " + boolToString(ato_->getPumpStatus()) + "\n\r";
  line = line + "Temperature: " + temperature_->getTemp() + "\n\r";

  return line;

}
/*



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
*/
