#include "Ato.h"
#include "Return.h"


void setup() 
{
  //Serial.begin( 9600 );
  //Serial.println( "Controller Starting Up..." );
}

void loop()
{
  Ato ato;
//  Return returnPump;
  while (true){
//    Serial.println( "Checking... ");
    ato.fullCheck();
//    Serial.println("");
//    delay(1000);
  }
}








