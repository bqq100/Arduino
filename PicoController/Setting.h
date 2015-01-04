#ifndef Setting_h
#define Setting_h

#include <Arduino.h>

//Commamd module default options
const int DEF_UPDATE_FREQ = 5;  //seconds
const int DEF_MAX_UPDATE_TIME = 168; //hours

class Setting{
  public :
    Setting();
    int    get( String variable );
 //   float get( String variable );
    bool   set( String variable, int value );
 //   float set( String variable, int value );
  private :
    int updateFreq_;
    int maxUpdateTime_;
};
 
#endif
