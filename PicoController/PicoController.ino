#include <OneWire.h>
#include "Utilities.h"
#include "Setting.h"
#include "Return.h"
#include "Temp.h"
#include "Command.h"
#include "Ato.h"

void setup() 
{
}

void loop()
{
  Setting settings;
  Ato ato;
  Return returnPump;
  Temp temperature;
  Command command(&settings, &ato, &temperature, &returnPump);
    
  while (true){
    ato.check();
    temperature.check();
    command.check();
    if ( freeRam() < 1024 )
      resetFunc(); //call reset
  }
}








