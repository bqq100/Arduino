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
#include "Waterchange.h"

// Message Defines
/*
#define STARTUP_MSG       F("Start...")
#define TIME_MSG          F("Time: ")
#define LO_SWITCH_MSG     F("Low: ")
#define HI_SWITCH_MSG     F("High: ")
#define ATO_STATUS_MSG    F("ATO: ")
#define RETURN_STATUS_MSG F("Return: ")
#define TEMP_STATUS_MSG   F("Temp: ")
#define HEAT_STATUS_MSG   F("Heater: ")
#define DOSER_STATUS_MSG  F("Doser: ")
#define WC_STATUS_MSG     F("WC: ")
#define LIGHT_STATUS_MSG  F("Light : ")
#define FREE_MEMORY_MSG   F("Mem: ")
#define LO_ALARM_MSG      F("ALARM: LOW")
#define HI_ALARM_MSG      F("ALARM: HIGH")
#define PUMP_ALARM_MSG    F("ALARM: PUMP")
*/

#define STARTUP_MSG       F("Controller Starting Up...")
#define TIME_MSG          F("Time             : ")
#define LO_SWITCH_MSG     F("Low Switch       : ")
#define HI_SWITCH_MSG     F("High Switch      : ")
#define ATO_STATUS_MSG    F("ATO Status       : ")
#define RETURN_STATUS_MSG F("Return Status    : ")
#define TEMP_STATUS_MSG   F("Temperature      : ")
#define HEAT_STATUS_MSG   F("Heater Status    : ")
#define DOSER_STATUS_MSG  F("Doser Status     : ")
#define WC_STATUS_MSG     F("WC Status        : ")
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
    Command(Setting*, Clock*, Light*, Doser*, Ato*, Temp*, Return*, Waterchange*);
    Command(Setting*, Clock*, Ato*, Light*, Return*, Temp*, Doser*, Waterchange*);
    void check();

    void output( char* );
    void output( String );
    void output( const __FlashStringHelper*, int    , bool );
    void output( const __FlashStringHelper*, bool   , bool );
    void output( const __FlashStringHelper*, float  , bool );
    void output( const __FlashStringHelper*, String , bool );
    void output( const __FlashStringHelper*         );
    
  private :
    Waterchange*    wc_;
    Setting*        settings_;
    Light*          light_;
    Doser*          doser_;
    Ato*            ato_;
    Temp*           temperature_;
    Return*         returnPump_;
    Clock*          clock_;
    char*           string_;
    unsigned long   nextStatus_;
    unsigned long   autoStatus_;

    void readChar();

    bool isValidChar( int );

    void processCommand();

    template <class T>
    void getLightStatus( char*, char*, T, const __FlashStringHelper* );

    template <class T> 
    void getEquipStatus( char*, char*, T, const __FlashStringHelper* );

    template <class T>
    void getWCStatus( char*, char*, T, const __FlashStringHelper* );

    void getClock  ( char*, char* );    
    void getStatus ( char*, char* );
    void getMemory ( char*, char* );
    void getSetting( char*, char* );
    void setSetting( char*, char* );
    void getAllSettings( char*, char* );
    
    bool stringsEqual(char*, const char*);
    bool stringStartsWith(char*, const char*);
    
};
 
#endif
