#ifndef Command_h
#define Command_h

#include <Arduino.h>
#include "Setting.h"
#include "Equip.h"
#include "Module.h"
#include "Light.h"
#include "Doser.h"
#include "Ato.h"
#include "Temp.h"
#include "Return.h"
#include "Clock.h"
#include "Utilities.h"

// Message Defines
#define STARTUP_MSG       F("Controller Starting Up...")
#define TIME_MSG          F("Time             : ")
#define LO_SWITCH_MSG     F("Low Switch       : ")
#define HI_SWITCH_MSG     F("High Switch      : ")
#define ATO_STATUS_MSG    F("ATO Status       : ")
#define RETURN_STATUS_MSG F("Return Status    : ")
#define TEMP_STATUS_MSG   F("Temperature      : ")
#define HEAT_STATUS_MSG   F("Heater Status    : ")
#define DOSER_STATUS_MSG  F("Doser Status     : ")
#define LIGHT_STATUS_MSG  F("Light Brightness : ")
#define FREE_MEMORY_MSG   F("Free Memory      : ")
#define LO_ALARM_MSG      F("ALARM: LOW WATER ALARM IS ON ( Low switch has been on too long... )")
#define HI_ALARM_MSG      F("ALARM: HIGH WATER ALARM IS ON ( High switch is on... )")
#define PUMP_ALARM_MSG    F("ALARM: PUMP ALARM IS ON ( Pump has run too long without filling... )")

static prog_char UPDATE_FREQ_NAME[] PROGMEM = "UpdateFreq";
static prog_char UPDATE_FREQ_DESC[] PROGMEM = "Auto Status Update Frequency";
static prog_char UPDATE_FREQ_UNIT[] PROGMEM = "secs";

static prog_char MAX_UPDATE_NAME[] PROGMEM = "MaxUpdateTime";
static prog_char MAX_UPDATE_DESC[] PROGMEM = "Auto Status Max Run Time";
static prog_char MAX_UPDATE_UNIT[] PROGMEM = "hrs";

class Command{
  public :
    Command(Setting*, Clock*, Light*, Doser*, Ato*, Temp*, Return*);
    Command(Setting*, Clock*);
    void check( float );

    void output( String );
    void output( const __FlashStringHelper*, int    , bool );
    void output( const __FlashStringHelper*, bool   , bool );
    void output( const __FlashStringHelper*, float  , bool );
    void output( const __FlashStringHelper*, String , bool );
    void output( const __FlashStringHelper*         );
    
  private :
    Setting*        settings_;
    Light*          light_;
    Doser*          doser_;
    Ato*            ato_;
    Temp*           temperature_;
    Return*         returnPump_;
    Clock*          clock_;
    String          input_;
    unsigned long   nextStatus_;
    unsigned long   autoStatus_;

    void readChar();

    bool isValidChar( int );

    void processCommand( String );

    void getClock  ( String, String );    
    void getLight  ( String, String );
    void getDoser  ( String, String );
    void getAto    ( String, String );
    void getStatus ( String, String );
    void getReturn ( String, String );
    void getMemory ( String, String );
    void getSetting( String, String );
    void setSetting( String, String );
    void getAllSettings( String, String );
    
};
 
#endif
