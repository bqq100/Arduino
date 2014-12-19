#include <OneWire.h>
#include "Ato.h"
#include "Return.h"
#include "Temp.h"

void setup() 
{
  Serial.begin( 9600 );
  Serial.println( "Controller Starting Up..." );
}

void loop()
{
  Ato ato;
  Return returnPump;
  Temp temperature;
  while (true){
    ato.fullCheck();
    temperature.check();
  }
}








