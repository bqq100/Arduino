#include "Command.h"

Command::Command(Setting* settings, Clock* clock, Ato* ato, Temp* temperature, Return* returnPump){

  output( STARTUP_MSG );
  
  settings->init( &UPDATE_FREQ_NAME[0], &UPDATE_FREQ_DESC[0], &UPDATE_FREQ_UNIT[0], 5   );
  settings->init( &MAX_UPDATE_NAME[0] , &MAX_UPDATE_DESC[0] , &MAX_UPDATE_UNIT[0] , 168 );
 
  settings_    = settings;
  clock_       = clock;
  ato_         = ato;
  temperature_ = temperature;
  returnPump_  = returnPump;
  nextStatus_  = 0;
  autoStatus_  = 0;
  input_       = "";
}

Command::Command(Setting* settings, Clock* clock){
  settings_ = settings;
  clock_    = clock;
  nextStatus_ = 0;
  autoStatus_ = 0;
  input_ = "";
}

void Command::check(){
  if ( settings_->getError().length() > 0 )
    output( settings_->getError() );
  if ( autoStatus_ && nextStatus_ && millis() > nextStatus_ )
    getStatus(String(F("status")),"");
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
  if ( ascii == 92 || ascii == 47 )
    return true;
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
  if ( command.startsWith( "set" ) )
    setSetting( command, option );
  if ( command.startsWith( "get" ) && command != "getAll" )
    getSetting( command, option );
  if ( command == "memory" )
    getMemory( command, option );
  if ( command == "reset" )
    resetFunc();
  if ( command == "getAll" )
    getAllSettings( command, option );
  if ( command == "ato" )
    getAto( command, option );
  if ( command == "return" )
    getReturn( command, option );
  if ( command == "clock" )
    getClock( command, option );
    
}

void Command::getClock( String command, String option ){
  if ( option.indexOf(":") > 0 )
    clock_->setTime( option );
  if ( option.indexOf("/") > 0 || option.indexOf("\\") > 0 )
    clock_->setDate( option );
  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString() );
}

void Command::getAto( String command, String option ){
  if ( option == "enable" )
    ato_->enable();
  if ( option == "disable" )
    ato_->disable(); 
  output( ATO_STATUS_MSG, ato_->getStatus() );
}

void Command::getReturn( String command, String option ){
  if ( option == "enable" )
    returnPump_->enable();
  if ( option == "disable" )
    returnPump_->disable();
  output( RETURN_STATUS_MSG, returnPump_->getPumpStatus() );
}

void Command::output(String value){
  Serial.println(value);
}

void Command::getSetting( String command, String option ){
  String setting,line;
  if ( command.startsWith("get") )
    setting = command.substring(3);
  else
    setting = command;
  line = settings_->getString( setting );
  output(line);
  return;
}

void Command::getAllSettings( String command, String option ){
  String thisSetting = settings_->getNextSetting( "" );
  while ( thisSetting.length() != 0 ){
    getSetting( thisSetting, "" );
    thisSetting = settings_->getNextSetting( thisSetting );
  }
  return;
}

void Command::setSetting( String command, String option ){
  String setting,line;
  if ( command.startsWith( "set" ) )
    setting = command.substring(3);
  else
    setting = command;  line = settings_->set( setting, option.toFloat() );
  output(line);
  return;
}

void Command::getMemory(String command, String option ){
  output( FREE_MEMORY_MSG, freeRam() );
}

void Command::output(const __FlashStringHelper* message, int value){
  Serial.println( String(message) + String(value) );
}

void Command::output(const __FlashStringHelper* message, bool value){
  Serial.println( String(message) + String(boolToString(value)) );
}

void Command::output(const __FlashStringHelper* message, float value){
  Serial.println( String(message) + String(value) );
}

void Command::output(const __FlashStringHelper* message, String value){
  Serial.println( String(message) + value );
}

void Command::output(const  __FlashStringHelper* message){
  Serial.println( String(message) );
}

void Command::getStatus( String command, String option ){
  if ( option == "stop" ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( option == "start" )
    autoStatus_ = millis() + (unsigned long)settings_->get( &MAX_UPDATE_NAME[0] ) * 1000 * 60 * 60;

  if ( autoStatus_ )
    nextStatus_ = millis() + (unsigned long)settings_->get( &UPDATE_FREQ_NAME[0] ) * 1000;

  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString() );
  output( ATO_STATUS_MSG,  ato_->getStatus() );
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
