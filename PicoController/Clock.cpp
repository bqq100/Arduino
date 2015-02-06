#include "Clock.h"

Clock::Clock( Setting* settings ){
  tmElements_t time;

  settings_ = settings;
  prevEpochTick_ = 0;
  epoch_ = 0;

  bool timeRead = RTC.read(time);
  rtcMode_ = RTC.chipPresent();
  
  if ( !timeRead )
    initClk();
}

void Clock::check(){
  unsigned long nowMillis = millis();
  bool tickNow = false;
  if ( prevEpochTick_ >= 4294966295 && nowMillis < 1000 && nowMillis > prevEpochTick_ - 4294966295 )
    tickNow = true;
  else if ( nowMillis > prevEpochTick_ + 1000 )
    tickNow = true;
    
  if ( tickNow ){
    epoch_++;
    prevEpochTick_ += 1000;
  }
} 

unsigned long Clock::getEpoch(){
  return epoch_;
}

float Clock::getTime(){
  return timeToFloat( getRtcTime() );
}

String Clock::formatString( int number ){
  if ( number < 10 )
    return "0" + String( number );
  else
    return String( number );  
}

String Clock::getTimeString(){
  tmElements_t time = getRtcTime();
  return formatString( time.Hour ) + ":" + formatString( time.Minute ) + ":" + formatString( time.Second );
}

String Clock::getDateString(){
  tmElements_t date = getRtcTime();
  return formatString( date.Month ) + "/" + formatString( date.Day ) + "/" + formatString( date.Year + 1970 ); 
}

int Clock::getWeekday(){
  return calcWeekday( getRtcTime() );
}

void Clock::setHour( String hour ){
  tmElements_t time = getRtcTime();
  time.Hour = hour.toInt();
  setRtcTime( time );
}

void Clock::setMinute( String minute ){
  tmElements_t time = getRtcTime();
  time.Minute = minute.toInt();
  setRtcTime( time );
}

void Clock::setSecond( String second ){
  tmElements_t time = getRtcTime();
  time.Second = second.toInt();
  setRtcTime( time );
}

void Clock::setTime( char* timeParam ){
  String newTime = String(timeParam);
  int firstSeperator = newTime.indexOf(':');
  int secondSeperator = newTime.lastIndexOf(':');
  String hour = newTime.substring(0,firstSeperator);
  String minute = newTime.substring(firstSeperator + 1, secondSeperator);
  String second = newTime.substring(secondSeperator + 1);
  tmElements_t time = getRtcTime();
  time.Hour = hour.toInt();
  time.Minute = minute.toInt();
  time.Second = second.toInt();
  setRtcTime( time );
}

void Clock::setMonth( String month ){
  tmElements_t time = getRtcTime();
  time.Month = month.toInt();
  setRtcTime( time );
}

void Clock::setDay( String day ){
  tmElements_t time = getRtcTime();
  time.Day = day.toInt();
  setRtcTime( time );
}

void Clock::setYear( String year ){
  int numericYear = year.toInt();
  if ( numericYear < 100 )
    numericYear += 2000;
  tmElements_t time = getRtcTime();
  time.Year = numericYear - 1970;
  setRtcTime( time );
}

void Clock::setDate( char* dateParam ){
  String newDate = String( dateParam );

  int firstSeperator;
  int secondSeperator;
  if ( newDate.indexOf('/') > 0 ){
    firstSeperator = newDate.indexOf('/');
    secondSeperator = newDate.lastIndexOf('/');
  }else{
    firstSeperator = newDate.indexOf("\\");
    secondSeperator = newDate.lastIndexOf("\\");
  }

  String month = newDate.substring(0,firstSeperator);
  String day   = newDate.substring(firstSeperator + 1, secondSeperator);
  String year  = newDate.substring(secondSeperator + 1);
  int numericYear = year.toInt();
  if ( numericYear < 100 )
    numericYear += 2000;

  tmElements_t time = getRtcTime();
  time.Month = month.toInt();
  time.Day = day.toInt();
  time.Year = numericYear - 1970;

  setRtcTime( time );
}




void Clock::initClk(){
  tmElements_t initTime;  
  initTime.Second = 0;
  initTime.Minute = 0;
  initTime.Hour   = 0;
  initTime.Month  = 1;
  initTime.Day    = 1;
  initTime.Year   = 45;
  setRtcTime( initTime );
}

void Clock::setRtcTime( tmElements_t time ){
  if ( rtcMode_ ){
    RTC.write( time );
  }else{
    startMillis_ = getEpoch();
    startMillisTime_ = time;
  }
}

tmElements_t Clock::getRtcTime(){
  tmElements_t time;
  RTC.read(time);
  if ( rtcMode_ )
    return time ;
  else
    return calcMillisTime();
}

int Clock::calcWeekday( tmElements_t time ){
  int d = (int) time.Day;
  int m = (int) time.Month;
  int y = (int) time.Year + 1970; 
  return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 + 1  ;
}

float Clock::timeToFloat( tmElements_t time ){
  return (float) time.Hour + (float) time.Minute / 60 + (float) time.Second / 60 / 60;
}

bool Clock::isLeapYear( int year ){
  return year%4 == 0 && (year %100 != 0 || year%400 == 0);
}

tmElements_t Clock::calcMillisTime(){
  tmElements_t time = startMillisTime_;
  unsigned long offset = getEpoch() - startMillis_;
  time.Second += ( offset % 60  );
  time.Minute += ( offset % 360 )  / 60;
  time.Hour   += ( offset % 8640 ) / 360;

  if ( time.Second >= 60 ){
    time.Second = time.Second - 60;
    time.Minute++;
  }

  if ( time.Minute >= 60 ){
    time.Minute = time.Minute - 60;
    time.Hour++;
  }
  
  if ( time.Hour >= 24 ){
    time.Day  = time.Day + time.Hour / 24;
    time.Hour = time.Hour % 24;
  }

  if ( time.Month == 2 && time.Day > 29 && isLeapYear( time.Year + 1970 ) ){
    time.Day = time.Day - 29;
    time.Month++;
  } 

  if ( time.Month == 2 && time.Day > 28 && !isLeapYear( time.Year + 1970) ){
    time.Day = time.Day - 28;
    time.Month++;
  }

  if ( time.Day > 31 && ( time.Month == 1 || time.Month == 3 || time.Month == 5 || time.Month == 7 || time.Month == 8 || time.Month == 10 || time.Month == 12 ) ){ 
    time.Day = time.Day - 31;
    time.Month++; 
  }

  if ( time.Day > 30 && ( time.Month == 4 || time.Month == 6 || time.Month == 9 || time.Month == 11 ) ){
    time.Day = time.Day - 30;
    time.Month++;
  }

  if ( time.Month > 12 ){
    time.Month = time.Month - 12;
    time.Year++;
  }

  return time;
}
