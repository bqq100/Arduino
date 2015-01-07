#ifndef Setting_h
#define Setting_h

#include <avr/pgmspace.h>

#include <Arduino.h>

struct Node {
  prog_char* name;
  prog_char* desc;
  prog_char* unit;
  float  value;
  Node*  next;
};

class Setting{
  public :
    Setting();
    float   get( String );
    String  set( String, float );
    String  getString( String );
  private :
    Node* firstNode_;
    void   init( prog_char* name, prog_char* description, float value );
    Node*  findNode( String );
    bool   compare( String, prog_char* );
    String nodeDescription( Node*);
};
 
#endif
