#include "Command.h"

Command::Command(){
  Serial.begin( 9600 );
  Serial.println( "Controller Starting Up..." );
  command_ = "";
  line_ = "";
}

void Command::check(){
  char c;
  while ( Serial.available() > 0 ){
    c = Serial.read();
    Serial.print(c);
    if ( c == '\n' || c == '\r' ){
      if ( command_.length() > 0 )
      processCommand();
    }
    else
      command_ = command_ + c;      
  }
}

void Command::processCommand(){
  line_ = "The command was: " + command_;
  line_ = line_ + " (" + command_.length() + ")";
  line_ = line_ + '\n';
  bool test = ato_.quickLoCheck();
  line_ = line_ + boolToString(test);
  output();
  line_ = "";
  command_ = "";
}

void Command::output(){
  Serial.println(line_);
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
