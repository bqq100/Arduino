#include <OneWire.h>
#include "Utilities.h"
#include "Setting.h"
#include "Return.h"
#include "Temp.h"
#include "Command.h"
#include "Ato.h"

void setup() 
{
  Serial.begin( 9600 );
}

void loop()
{
  Setting settings;
  Ato ato(&settings);
  Return returnPump(&settings);
  Temp temperature(&settings);
  Command command(&settings, &ato, &temperature, &returnPump);
    
  while (true){
    ato.check();
    temperature.check();
    command.check();
    if ( freeRam() < 1024 )
      resetFunc(); //call reset
  }
}








