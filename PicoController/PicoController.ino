#include <OneWire.h>
#include "Utilities.h"
#include "Return.h"
#include "Temp.h"
#include "Command.h"
#include "Ato.h"

void setup() 
{
}

void loop()
{
  Ato ato;
  Return returnPump;
  Temp temperature;
  Command command(&ato, &temperature, &returnPump);
    
  while (true){
    ato.check();
    temperature.check();
    command.check();
  }
}








