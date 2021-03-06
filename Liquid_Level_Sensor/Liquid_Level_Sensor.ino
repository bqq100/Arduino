#include "Ato.h"
#include <NewPing.h>
#include <EEPROM.h>

unsigned long previousMillis = 0;
unsigned long interval = 1000;

void setup() {
 Serial.begin( 9600 );
 Serial.println( "Liquid Level Detector" );
}

void loop()
{
  Ato ato;
  char line[100];
  while (true){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      ato.readSensor( line );
      Serial.println( line );
    } 
    ato.readCommand();

    if ( ato.getPumpFlag() )
      sprintf( line, "PUMP ON" );
    if ( ato.getLowFlag() )
      sprintf( line, "ERROR: Water too low (%d mm)", ato.getLevel() );
    if ( ato.getHighFlag() )
      sprintf( line, "ERROR: Water too high (%d mm)", ato.getLevel() );
    if ( ato.getErrorFlag() )
      sprintf( line, "ERROR: Generic Error Detected" );
    if ( ato.getPumpFlag() || ato.getLowFlag() || ato.getHighFlag() || ato.getErrorFlag() )
      Serial.println(line);
  }
}


