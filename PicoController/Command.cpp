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

void Command::check(){
  
  if ( autoStatus_ && nextStatus_ && millis() > nextStatus_ )
    output(status("status",""));
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
    output( status( command, option ) );
  if ( command.startsWith("set") );
    output( setSetting( command, option ) );
  if ( command.startsWith("get") );
    output( getSetting( command, option ) );
}

void Command::output(String outputLine){
  Serial.print(outputLine);
}

String Command::getSetting( String command, String option ){
  String line;
  if ( command == "getUpdateFreq" || command == "getAll" )
    line = line + UPDATE_FREQ_MSG + settings_->get( "updateFreq" ) + F("secs") + NEWLINE;
  if ( command == "getMaxAutoUpdate" || command == "getAll" )
    line = line + MAX_DURATION_MSG + settings_->get( "maxUpdateTime" ) + F("hrs") + NEWLINE;
  return line;
}

String Command::setSetting( String command, String option ){
  String line;
  if ( command == "setUpdateFreq" ){
    settings_->set( "updateFreq", (int)option.toInt() );
    line = getSetting("getUpdateFreq", "");
  }
  if ( command == "setMaxAutoUpdate" ){
    settings_->set( "maxUpdateTime", (int)option.toInt() );
    line = getSetting("getMaxAutoUpdate", "");
  }

  return line;
}

String Command::status( String command, String option ){

  if ( option == "stop" ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( option == "start" )
    autoStatus_ = millis() + (unsigned long)settings_->get("maxUpdateTime") * 1000 * 60 * 60;

  if ( autoStatus_ )
    nextStatus_ = millis() + (unsigned long)settings_->get("updateFreq") * 1000;

  String line;
  line = line + LO_SWITCH_MSG   + boolToString(ato_->quickLoCheck()) + NEWLINE;
  line = line + HI_SWITCH_MSG   + boolToString(ato_->quickHiCheck()) + NEWLINE;
  line = line + PUMP_STATUS_MSG + boolToString(ato_->getPumpStatus()) + NEWLINE;
  line = line + TEMP_STATUS_MSG + temperature_->getTemp() + NEWLINE;
  line = line + HEAT_STATUS_MSG + boolToString(temperature_->getHeaterStatus()) + NEWLINE;
  line = line + FREE_MEMORY_MSG + freeRam() + NEWLINE;
  if ( ato_->getLoAlarm() )
    line = line + LO_ALARM_MSG + NEWLINE;
  if ( ato_->getHiAlarm() )
    line = line + HI_ALARM_MSG + NEWLINE;
  if ( ato_->getPumpAlarm() )
    line = line + PUMP_ALARM_MSG + NEWLINE;

  return line;

}
