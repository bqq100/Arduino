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
#include "Light.h"
#include "Doser.h"

#define ATO_PUMP_PIN   13
#define ATO_HI_PIN     9
#define ATO_LO_PIN     8
#define RETURN_PIN     11
#define CH1_DIMMER_PIN 7
#define CH2_DIMMER_PIN 6
#define TEMP_PIN   10
#define HEATER_PIN 12
#define DOSER_PIN  5

void setup() 
{
}

void loop()
{
  Setting settings;
  Clock clock(&settings);
  Ato    ato        (&settings, ATO_PUMP_PIN  , ATO_HI_PIN    , ATO_LO_PIN);
  Light  light      (&settings, CH1_DIMMER_PIN, CH2_DIMMER_PIN            );  
  Temp   temperature(&settings, HEATER_PIN    , TEMP_PIN                  );
  Doser  doser      (&settings, DOSER_PIN                                 );
  Return returnPump (&settings, RETURN_PIN                                );
  Command command   (&settings, &clock, &light, &doser, &ato, &temperature, &returnPump   );
    
  while (true){
    light.check( clock.getTime() );
    doser.check( clock.getTime() );
    ato.check  ( clock.getTime() );
    returnPump.check ( clock.getTime() );
    temperature.check( clock.getTime() );
    command.check    ( clock.getTime() );
    if ( freeRam() < 512 )
      resetFunc(); //call reset
  }
}
