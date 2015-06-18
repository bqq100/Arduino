#include "Command.h"

Command::Command(Setting* settings, Clock* clock, Ato* ato, Light* light, Return* returnPump, Temp* temperature, Doser* doser, Waterchange* wc){

  Serial.begin(9600);

  output( STARTUP_MSG );
  
  settings->init( &UPDATE_FREQ_NAME[0], &UPDATE_FREQ_DESC[0], &UPDATE_FREQ_UNIT[0], 5   );
  settings->init( &MAX_UPDATE_NAME[0] , &MAX_UPDATE_DESC[0] , &MAX_UPDATE_UNIT[0] , 168 );
  string_ = new char[MAX_SIZE];
  *string_ = (char) 0;
  settings_ = settings;
  clock_    = clock;
  ato_      = ato;
  light_    = light;
  returnPump_ = returnPump;
  temperature_ = temperature;
  doser_       = doser;
  wc_          = wc;
  nextStatus_ = 0;
  autoStatus_ = 0;
}

void Command::check(){
  unsigned long currentEpoch = clock_->getEpoch();
  if ( settings_->getError().length() > 0 )
    output( settings_->getError() );
  if ( autoStatus_ && nextStatus_ && currentEpoch >= nextStatus_ )
    getStatus("status","");
  if ( autoStatus_ && currentEpoch > autoStatus_ )
    autoStatus_ = 0;
  
  while ( Serial.available() > 0 )
    readChar();
}

void Command::readChar(){
    char c = Serial.read();
    if ( c == '\n' || c == '\r' ){
      if ( strlen(string_) == 0 )
        strcpy( string_, "status" );
      processCommand();
      *string_ = (char) 0;
    }
    else{
      int ascii = (int)c;
      if ( isValidChar(ascii) )
        strncat(string_, &c, 1);
	  if ( *string_ == '0' && c == ' ' && strlen(string_) == 1 ){
        Serial.write(0x14);  // reply two char to avrdude.exe
        Serial.write(0x10);  //  for synchronization 
        resetFunc(); 
      }else if ( ( ascii == 127 || ascii == 8 ) && strlen(string_) > 0 ){
        *(string_ + strlen(string_) - 1) = 0;
      }
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

bool Command::stringsEqual(char* string1, const char* string2){
  bool match = true;
  unsigned int i;
  for (i = 0; i < strlen(string1) && *(string1 + i) != ':'; i++){
    if ( *(string1 + i) != *(string2+i) )
      match = false;
  }
  if ( strlen(string2) == i )
    return match;
  else
    return false;
}

bool Command::stringStartsWith(char* string1, const char* string2){
  bool match = true;
  for (unsigned int i = 0; i < strlen(string2); i++){
    if( *(string1 + i) != *(string2+i) )
      match = false;
  }
  return match;
}

        
void Command::processCommand(){
  char* command;
  char* option;

  command = string_;
  if ( strchr(string_, ':') != NULL )
    option = strchr(string_, ':') + 1;
  else
    option = 0;

  if ( stringsEqual(command, "status") )
    getStatus( command, option );
  if ( stringStartsWith(command, "set") ) // 1-2kb
    setSetting( command, option );
  if ( stringStartsWith(command, "get") && !stringsEqual(command, "getAll") )
    getSetting( command, option );
  if ( stringsEqual(command, "memory") )
    getMemory( command, option );
  if ( stringsEqual(command, "reset") )
    resetFunc();
  if ( stringsEqual(command, "getAll") )  // 1-2kB
    getAllSettings( command, option );
  if ( stringsEqual(command, "clock") )  // 2kB
    getClock( command, option );
  if ( stringsEqual(command, "light") )
    getLightStatus( command, option, light_, LIGHT_STATUS_MSG );
  if ( stringsEqual(command, "ato") )
    getEquipStatus( command, option, ato_, ATO_STATUS_MSG );
  if ( stringsEqual(command, "return") )
    getEquipStatus( command, option, returnPump_, RETURN_STATUS_MSG );
  if ( stringsEqual(command, "doser") )
    getEquipStatus( command, option, doser_, DOSER_STATUS_MSG );
  if ( stringsEqual(command, "heater") )
    getEquipStatus( command, option, temperature_, HEAT_STATUS_MSG );
  if ( stringsEqual(command, "waterchange") || stringsEqual(command, "wc") )
    getWCStatus( command, option, wc_, WC_STATUS_MSG );
    
}

template <class T>
void Command::getLightStatus( char* command, char* option, T equip, const __FlashStringHelper* message ){
  if ( stringsEqual(option, "auto") )
    equip->enable();
  if ( stringsEqual(option, "off") )
    equip->disable();
  if ( stringsEqual(option, "on") )
    equip->forceOn();
  else if ( strlen(option) > 0 )
    equip->lightOn( option );
  equip->check();
  output( message, equip->getEquipStatus(), equip->getStatus() );
}

template <class T> 
void Command::getEquipStatus( char* command, char* option, T equip, const __FlashStringHelper* message ){
  if ( stringsEqual(option, "auto") )
    equip->enable();
  if ( stringsEqual(option, "off") )
    equip->disable(); 
  if ( stringsEqual(option, "on") )
    equip->forceOn();
  equip->check();
  output( message, equip->getEquipStatus(), equip->getStatus() );
}  

template <class T>
void Command::getWCStatus( char* command, char* option, T equip, const __FlashStringHelper* message ){
  if ( stringsEqual(option, "auto") )
    equip->enable();
  if ( stringsEqual(option, "off") )
    equip->disable();
  if ( stringsEqual(option, "on") )
    equip->forceOn();
  equip->check();
  output( message, equip->getWCStatus(), equip->getStatus() );
}

void Command::getClock( char* command, char* option ){
  if ( strchr(option, ':') > 0 )
    clock_->setTime( option );
  if ( strchr(option, '/') > 0 || strchr(option, '\\') > 0 )
    clock_->setDate( option );
  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString(), true );
}

void Command::output(char* value){
  Serial.println(value);
}

void Command::output(String value){
  Serial.println(value);
}

void Command::getSetting( char* command, char* option ){
  char* setting = command;
  if ( stringStartsWith(command, "get") )
    setting += 3;
  settings_->getString( setting );
  output(setting);
  return;
}

void Command::getAllSettings( char* command, char* option ){
  char* setting = command;
  char* temp = new char[MAX_SIZE];
  *temp = (char) 0;
  settings_->getNextSetting( temp );
  while ( strlen(temp) != 0 ){
    strcpy(setting, temp);
    getSetting( setting, 0 );
    settings_->getNextSetting( temp );
  }
  delete(temp);
  return;
}

void Command::setSetting( char* command, char* option ){

  char* setting = command;
  if ( stringStartsWith(command, "set") )
    setting += 3;

  if ( option && *(option - 1) == ':' ){
    float value = atof(option);
    *(option - 1) = (char) 0;
    settings_->set( setting, value );
  }else{
    settings_->getString( setting );
  }
  output(setting);
  return;
}

void Command::getMemory(char* command, char* option ){
  output( FREE_MEMORY_MSG, freeRam(), true );
}

void Command::output(const __FlashStringHelper* message, int value, bool status){
  output( message, String(value), status );
}

void Command::output(const __FlashStringHelper* message, bool value, bool status){
  output( message, String(boolToString(value)), status );
}

void Command::output(const __FlashStringHelper* message, float value, bool status){
  output( message, String(value), status );
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

void Command::getStatus( char* command, char* option ){
  unsigned long currentEpoch = clock_->getEpoch();
  
  if ( stringsEqual(option, "stop") ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( stringsEqual(option, "start") )
    autoStatus_ = currentEpoch + (unsigned long)settings_->get( &MAX_UPDATE_NAME[0] ) * 60 * 60;
  
  if ( autoStatus_ )
    nextStatus_ = currentEpoch + (unsigned long)settings_->get( &UPDATE_FREQ_NAME[0] );

  output( TIME_MSG, clock_->getDateString() + " " + clock_->getTimeString(), true );
  output( LIGHT_STATUS_MSG,  light_->getEquipStatus()      , light_->getStatus()       );
  output( LO_SWITCH_MSG,     ato_->quickLoCheck()          , true                      );
  output( HI_SWITCH_MSG,     ato_->quickHiCheck()          , true                      );
  output( ATO_STATUS_MSG,    ato_->getEquipStatus()        , ato_->getStatus()         );
  output( TEMP_STATUS_MSG,   temperature_->getTemp()       , true                      );
  output( HEAT_STATUS_MSG,   temperature_->getEquipStatus(), temperature_->getStatus() );
  output( RETURN_STATUS_MSG, returnPump_->getEquipStatus() , returnPump_->getStatus()  );
  output( DOSER_STATUS_MSG,  doser_->getEquipStatus()      , doser_->getStatus()       );
  output( WC_STATUS_MSG,     wc_->getWCStatus()            , wc_->getStatus()          );
  output( FREE_MEMORY_MSG,   freeRam()                     , true                      );

  if ( ato_->getLoAlarm() )
    output( LO_ALARM_MSG );
  if ( ato_->getHiAlarm() )
    output( HI_ALARM_MSG );
  if ( ato_->getPumpAlarm() )
    output( PUMP_ALARM_MSG );
  
  return;
}

