#include "Command.h"

Command::Command(Setting* settings, Ato* ato, Temp* temperature, Return* returnPump){
  Serial.begin( 9600 );
  Serial.println( STARTUP_MSG );
  settings_ = settings;
  ato_ = ato;
  temperature_ = temperature;
  returnPump_ = returnPump;
  nextStatus_ = 0;
  autoStatus_ = 0;
  input_ = "";
}

Command::Command(Setting* settings){
  settings_ = settings;
  nextStatus_ = 0;
  autoStatus_ = 0;
  input_ = "";
}

void Command::check(){
  if ( autoStatus_ && nextStatus_ && millis() > nextStatus_ )
    getStatus("status","");
  if ( autoStatus_ && millis() > autoStatus_ )
    autoStatus_ = 0;
  
  while ( Serial.available() > 0 )
    readChar();
}

void Command::readChar(){
    char c = Serial.read();
    if ( c == '\n' || c == '\r' ){
      if ( input_.length() > 0 ){
        input_.trim();
        processCommand( input_ );
        input_ = "";
      }
    }
    else{
      int ascii = (int)c;
      if ( isValidChar(ascii) )
        input_ = input_ + c;
    }
} 

bool Command::isValidChar( int ascii ){
  if ( ascii < 48 )
    return false;
  if ( ascii > 58 && ascii < 65 )
    return false;
  if ( ascii > 90 && ascii < 97 )
    return false;
  if ( ascii > 122 )
    return false;
  return true;
}
        
void Command::processCommand( String input ){
  String command, option;

  if ( input.indexOf(':') > 0 ){
    command = input.substring( 0, input.indexOf(':') );
    option  = input.substring( input.indexOf(':') + 1 );
  }else{
    command = input;
    option = "";
  }

  if ( command == "status" )
    getStatus( command, option );
  if ( command.startsWith("set") )
    setSetting( command, option );
  if ( command.startsWith("get") )
    getSetting( command, option );
  if ( command == "memory" )
    getMemory( command, option );
  if ( command == "reset" )
    resetFunc();
    
}

void Command::output(String outputLine){
  Serial.print(outputLine);
}

void Command::getSetting( String command, String option ){
  String setting,line;
  setting = command.substring(3);
  line = settings_->getString( setting );
  output(line);
  return;
}

void Command::setSetting( String command, String option ){
  String setting,line;
  setting = command.substring(3);
  line = settings_->set( setting, option.toFloat() );
  output(line);
  return;
}

void Command::getMemory( String command, String option ){
  output( FREE_MEMORY_MSG, freeRam() );
}

void Command::output(const  __FlashStringHelper* message, int value){
  Serial.print(message);
  Serial.println(value);
}

void Command::output(const  __FlashStringHelper* message, bool value){
  Serial.print(message);
  Serial.println(boolToString(value));
}

void Command::output(const  __FlashStringHelper* message, float value){
  Serial.print(message);
  Serial.println(value);
}

void Command::output(const  __FlashStringHelper* message){
  Serial.println(message);
}

void Command::getStatus( String command, String option ){
  if ( option == "stop" ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( option == "start" )
    autoStatus_ = millis() + (unsigned long)settings_->get("MaxUpdateTime") * 1000 * 60 * 60;

  if ( autoStatus_ )
    nextStatus_ = millis() + (unsigned long)settings_->get("UpdateFreq") * 1000;

  output( LO_SWITCH_MSG,   ato_->quickLoCheck() );
  output( HI_SWITCH_MSG,   ato_->quickHiCheck() );
  output( PUMP_STATUS_MSG, ato_->getPumpStatus() );
  output( TEMP_STATUS_MSG, temperature_->getTemp() );
  output( HEAT_STATUS_MSG, temperature_->getHeaterStatus() );
  output( FREE_MEMORY_MSG, freeRam() );

  if ( ato_->getLoAlarm() )
    output( LO_ALARM_MSG );
  if ( ato_->getHiAlarm() )
    output( HI_ALARM_MSG );
  if ( ato_->getPumpAlarm() )
    output( PUMP_ALARM_MSG );
  
  return;
}
