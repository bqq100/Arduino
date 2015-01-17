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


#define ATO_PUMP_PIN 13
#define ATO_HI_PIN   9
#define ATO_LO_PIN   8
#define RETURN_PIN   11

void setup() 
{
  Serial.begin( 9600 );
}

void loop()
{
  Setting settings;
  Clock clock(&settings);
  Ato ato(&settings, ATO_PUMP_PIN, ATO_HI_PIN, ATO_LO_PIN);
  Return returnPump(&settings, RETURN_PIN);
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








