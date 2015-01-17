#ifndef Clock_h
#define Clock_h

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Arduino.h>
#include "Setting.h" 

class Clock{
  public:
    Clock( Setting* );
    float  getTime();
    int    getWeekday();
    String getDateString();
    String getTimeString();
    void   setTime  ( String );
    void   setHour  ( String );
    void   setMinute( String );
    void   setSecond( String );
    void   setDate  ( String );
    void   setDay   ( String );
    void   setMonth ( String );
    void   setYear  ( String );
  private:
    Setting* settings_;
    bool     rtcMode_;
    tmElements_t startMillisTime_;
    unsigned long startMillis_;
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
