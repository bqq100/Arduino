#ifndef Setting_h
#define Setting_h

#include <avr/pgmspace.h>

#include <Arduino.h>

struct Node {
  prog_char* name;
  prog_char* desc;
//  String* unit;
  float  value;
  Node*  next;
};

class Setting{
  public :
    Setting();
    float   get( String variable );
    String  getString( String variable );
    String  set( String variable, float value );
  private :
    void init( prog_char* name, prog_char* description, float value );
    Node* firstNode_;
};
 
#endif
