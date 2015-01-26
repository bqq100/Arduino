#include "Command.h"

Command::Command(Setting* settings, Clock* clock, Light* light, Doser* doser, Ato* ato, Temp* temperature, Return* returnPump){
  
  Serial.begin(9600);
  output( STARTUP_MSG );
  
  settings->init( &UPDATE_FREQ_NAME[0], &UPDATE_FREQ_DESC[0], &UPDATE_FREQ_UNIT[0], 5   );
  settings->init( &MAX_UPDATE_NAME[0] , &MAX_UPDATE_DESC[0] , &MAX_UPDATE_UNIT[0] , 168 );
 
  settings_    = settings;
  clock_       = clock;
  light_       = light;
  doser_       = doser;
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
  Serial.begin(9600);
}

void Command::check( float currentTime ){
  if ( settings_->getError().length() > 0 )
    output( settings_->getError() );
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
  if ( command.startsWith( "set" ) ) // 1-2kb
    setSetting( command, option );
  if ( command.startsWith( "get" ) && command != "getAll" )
    getSetting( command, option );
  if ( command == "memory" )
    getMemory( command, option );
  if ( command == "reset" )
    resetFunc();
  if ( command == "getAll" )  // 1-2kB
    getAllSettings( command, option );
  if ( command == "ato" )
    getAto( command, option );
  if ( command == "return" )
    getReturn( command, option );
  if ( command == "clock" )  // 2kB
    getClock( command, option );
  if ( command == "light" )
    getLight( command, option );
  if ( command == "doser" )
    getDoser( command, option );
    
}

void Command::getClock( String command, String option ){
  if ( option.indexOf(":") > 0 )
    clock_->setTime( option );
  if ( option.indexOf("/") > 0 || option.indexOf("\\") > 0 )
    clock_->setDate( option );
  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString(), true );
}

void Command::getAto( String command, String option ){
  if ( option == "enable" )
    ato_->enable();
  if ( option == "disable" )
    ato_->disable(); 
  ato_->check( clock_->getTime() );
  output( ATO_STATUS_MSG, ato_->getEquipStatus(), ato_->getStatus() );
}

void Command::getReturn( String command, String option ){
  if ( option == "enable" )
    returnPump_->enable();
  if ( option == "disable" )
    returnPump_->disable();
  returnPump_->check( clock_->getTime() );
  output( RETURN_STATUS_MSG, returnPump_->getEquipStatus(), returnPump_->getStatus() );
}

void Command::getLight( String command, String option ){
  if ( option == "enable" )
    light_->enable();
  if ( option == "disable" )
    light_->disable();
  light_->check( clock_->getTime() );
  output( LIGHT_STATUS_MSG, light_->getBrightness(), light_->getStatus() );
}

void Command::getDoser( String command, String option ){
  if ( option == "enable" )
    doser_->enable();
  if ( option == "disable" )
    doser_->disable();
  doser_->check( clock_->getTime() );
  output( DOSER_STATUS_MSG, doser_->getEquipStatus(), doser_->getStatus() );
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
  output( FREE_MEMORY_MSG, freeRam(), true );
}

void Command::output(const __FlashStringHelper* message, int value, bool status){
  if ( status )
    Serial.println( String(message) + String(value) );
  else
    Serial.println( String(message) + "DISABLED" );
  
}

void Command::output(const __FlashStringHelper* message, bool value, bool status){
  if ( status )
    Serial.println( String(message) + String(boolToString(value)) );
  else
    Serial.println( String(message) + "DISABLED" );
}

void Command::output(const __FlashStringHelper* message, float value, bool status){
  if ( status )
    Serial.println( String(message) + String(value) );
  else
    Serial.println( String(message) + "DISABLED" );
}

void Command::output(const __FlashStringHelper* message, String value, bool status){
  if ( status )
    Serial.println( String(message) + value );
  else
    Serial.println( String(message) + "DISABLED" );
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

  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString(), true );
  output( LIGHT_STATUS_MSG,  light_->getBrightness()       , light_->getStatus()       );
  output( LO_SWITCH_MSG,     ato_->quickLoCheck()          , true                      );
  output( HI_SWITCH_MSG,     ato_->quickHiCheck()          , true                      );
  output( ATO_STATUS_MSG,    ato_->getEquipStatus()        , ato_->getStatus()         );
  output( TEMP_STATUS_MSG,   temperature_->getTemp()       , true                      );
  output( HEAT_STATUS_MSG,   temperature_->getEquipStatus(), temperature_->getStatus() );
  output( RETURN_STATUS_MSG, returnPump_->getEquipStatus() , returnPump_->getStatus()  );
  output( DOSER_STATUS_MSG,  doser_->getEquipStatus()      , doser_->getStatus()       );
  output( FREE_MEMORY_MSG,   freeRam()                     , true                      );

  if ( ato_->getLoAlarm() )
    output( LO_ALARM_MSG );
  if ( ato_->getHiAlarm() )
    output( HI_ALARM_MSG );
  if ( ato_->getPumpAlarm() )
    output( PUMP_ALARM_MSG );
  
  return;
}
