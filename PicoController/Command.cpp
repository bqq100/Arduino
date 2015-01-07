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
  Serial.begin( 9600 );
  Serial.println( STARTUP_MSG );
  settings_ = settings;
  nextStatus_ = 0;
  autoStatus_ = 0;
  input_ = "";
}

void Command::check(){
  Serial.print("PreCheck Free Mem: ");
  Serial.println(freeRam());
  
  if ( autoStatus_ && nextStatus_ && millis() > nextStatus_ )
    status("status","");
  if ( autoStatus_ && millis() > autoStatus_ )
    autoStatus_ = 0;
  
  while ( Serial.available() > 0 )
    readChar();
}

void Command::readChar(){
    Serial.print("Pre Read Char Free Mem: ");
    Serial.println(freeRam());
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
    Serial.print("Pre Process Free Mem: ");
    Serial.println(freeRam());

  String command, option;

  if ( input.indexOf(':') > 0 ){
    command = input.substring( 0, input.indexOf(':') );
    option  = input.substring( input.indexOf(':') + 1 );
  }else{
    command = input;
    option = "";
  }

  if ( command == "status" )
    status( command, option );
  if ( command.startsWith("set") )
    output( setSetting( command, option ) );
  if ( command.startsWith("get") )
    output( getSetting( command, option ) );
  if ( command == "reset" )
    resetFunc();

    Serial.print("Post Process Free Mem: ");
    Serial.println(freeRam());


}

void Command::output(String outputLine){
  Serial.print(outputLine);
}

String Command::getSetting( String command, String option ){
  String setting,line;
  setting = command.substring(3);
  line = settings_->getString( setting );
  return line;
}

String Command::setSetting( String command, String option ){
  String setting,line;
  setting = command.substring(3);
  line = settings_->set( setting, option.toFloat() );
  return line;
}

String Command::status( String command, String option ){
  Serial.print("Before Free Mem: ");
  Serial.println(freeRam());
  if ( option == "stop" ){
    autoStatus_ = 0;
    nextStatus_ = 0;
  }

  if ( option == "start" )
    autoStatus_ = millis() + (unsigned long)settings_->get("MaxUpdateTime") * 1000 * 60 * 60;

  if ( autoStatus_ )
    nextStatus_ = millis() + (unsigned long)settings_->get("UpdateFreq") * 1000;

  String line = "";
  line = line + LO_SWITCH_MSG   + boolToString(ato_->quickLoCheck()) + NEWLINE;
  output(line);
  line = "";
  line = line + HI_SWITCH_MSG   + boolToString(ato_->quickHiCheck()) + NEWLINE;
  output(line);
  line = "";
  line = line + PUMP_STATUS_MSG + boolToString(ato_->getPumpStatus()) + NEWLINE;
  output(line);
  line = "";
  line = line + TEMP_STATUS_MSG + temperature_->getTemp() + NEWLINE;
  output(line);
  line = "";
  line = line + HEAT_STATUS_MSG + boolToString(temperature_->getHeaterStatus()) + NEWLINE;
  output(line);
  line = "";
  line = line + FREE_MEMORY_MSG + freeRam() + NEWLINE;
  output(line);
  if ( ato_->getLoAlarm() ){
    line = "";
    line = line + LO_ALARM_MSG + NEWLINE;
    output(line);
  }
  if ( ato_->getHiAlarm() ){
    line = "";
    line = line + HI_ALARM_MSG + NEWLINE;
    output(line);
  }
  if ( ato_->getPumpAlarm() ){
    line = "";
    line = line + PUMP_ALARM_MSG + NEWLINE;
    output(line);
  }
  
  Serial.print("After Free Mem: ");
  Serial.println(freeRam());

  
  return line;

}
