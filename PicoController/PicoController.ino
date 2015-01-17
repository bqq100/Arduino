#include <OneWire.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include "Clock.h"
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
  Clock clock(&settings);
  Ato ato(&settings);
  Return returnPump(&settings);
  Temp temperature(&settings);
  Command command(&settings, &clock, &ato, &temperature, &returnPump);
    
  while (true){
    ato.check();
    returnPump.check();
    temperature.check();
    command.check();
    if ( freeRam() < 768 )
      resetFunc(); //call reset
  }
}








