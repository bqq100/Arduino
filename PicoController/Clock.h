#ifndef Clock_h
#define Clock_h

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Arduino.h>
#include "Setting.h" 
#include "Utilities.h"

class Clock{
  public:
    Clock( Setting* );
    void   check();
    float  getTime();
    int    getWeekday();
    String getDateString();
    String getTimeString();
    unsigned long getEpoch();
    void   setTime  ( char* );
    void   setHour  ( String );
    void   setMinute( String );
    void   setSecond( String );
    void   setDate  ( char* );
    void   setDay   ( String );
    void   setMonth ( String );
    void   setYear  ( String );
  private:
    Setting* settings_;
    bool     rtcMode_;
    tmElements_t startMillisTime_;
    unsigned long startMillis_;
    unsigned long epoch_;
    unsigned long prevEpochTick_;
    void   initClk();
    void setRtcTime( tmElements_t );
    tmElements_t getRtcTime();
    tmElements_t calcMillisTime();
    int   calcWeekday( tmElements_t );
    float timeToFloat( tmElements_t );
    bool  isLeapYear( int );
    String formatString( int );
};

#endif
