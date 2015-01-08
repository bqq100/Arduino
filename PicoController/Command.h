#ifndef Command_h
#define Command_h

#include <Arduino.h>
#include "Setting.h"
#include "Ato.h"
#include "Temp.h"
#include "Return.h"

// Message Defines
#define STARTUP_MSG       F("Controller Starting Up...")
#define LO_SWITCH_MSG     F("Low Switch    : ")
#define HI_SWITCH_MSG     F("High Switch   : ")
#define PUMP_STATUS_MSG   F("Pump Status   : ")
#define TEMP_STATUS_MSG   F("Temperature   : ")
#define HEAT_STATUS_MSG   F("Heater Status : ")
#define FREE_MEMORY_MSG   F("Free Memory   : ")
#define LO_ALARM_MSG      F("ALARM: LOW WATER ALARM IS ON ( Low switch has been on too long... )")
#define HI_ALARM_MSG      F("ALARM: HIGH WATER ALARM IS ON ( High switch is on... )")
#define PUMP_ALARM_MSG    F("ALARM: PUMP ALARM IS ON ( Pump has run too long without filling... )")

class Command{
  public :
    Command(Setting* settings, Ato* ato, Temp* temperature, Return* returnPump);
    Command(Setting* settings);
    void check();

    static void output( String );
    static void output( const __FlashStringHelper*, int   );
    static void output( const __FlashStringHelper*, bool  );
    static void output( const __FlashStringHelper*, float );
    static void output( const __FlashStringHelper*        );
    
  private :
    Setting*      settings_;
    Ato*          ato_;
    Temp*         temperature_;
    Return*       returnPump_;
    String        input_;
    unsigned long nextStatus_;
    unsigned long autoStatus_;

    void readChar();

    bool isValidChar( int );

    void processCommand( String );
    
    void getStatus ( String, String );
    void getMemory ( String, String );
    void getSetting( String, String );
    void setSetting( String, String );
    void getAllSettings( String, String );
    
};
 
#endif
