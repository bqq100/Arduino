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
#include "Waterchange.h"

// 12v Control Pins
#define ATO_PUMP_PIN   A0
#define DOSER_PIN      A1
#define FAN_PIN        A2
#define DRAIN_PIN      12
#define FILL_PIN       13 

// 120V Control Pins
#define RETURN_PIN     11
#define HEATER_PIN     10

// PWM Control Pins
#define CH1_DIMMER_PIN 5
#define CH2_DIMMER_PIN 6

// Input Pins
#define ATO_HI_PIN     7
#define ATO_LO_PIN     8
#define TEMP_PIN       9

void setup() 
{
}

void loop()
{
  Setting settings;
  Clock       clock       (&settings);
  Ato         ato         (&settings, &clock, ATO_PUMP_PIN   , ATO_HI_PIN    , ATO_LO_PIN );
  Waterchange wc          (&settings, &clock, &ato           , DRAIN_PIN      , FILL_PIN  );  
  Temp        temperature (&settings, &clock, HEATER_PIN     , TEMP_PIN                   );
  Doser       doser       (&settings, &clock, DOSER_PIN                                   );
  Light       light       (&settings, &clock, CH1_DIMMER_PIN , CH2_DIMMER_PIN, FAN_PIN    ); 
  Return      returnPump  (&settings, &clock, RETURN_PIN                                  );

  Command command (&settings, &clock, &ato, &light, &returnPump, &temperature, &doser, &wc);  

  while (true){
    clock.check();
    doser.check();
    ato.check  ();
    wc.check   ();
    returnPump.check ();
    temperature.check();
    light.check();
    command.check    (); 
    if ( freeRam() < 512 )
      resetFunc(); //call reset
  }
}
