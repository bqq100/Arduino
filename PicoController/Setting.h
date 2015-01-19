#ifndef Setting_h
#define Setting_h

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <stdlib.h>

const static int MAX_NAME_SIZE = 35;
const static int MAX_DESC_SIZE = 100;
const static int MAX_UNIT_SIZE = 10;
const static int MAX_SIZE      = MAX_NAME_SIZE + MAX_DESC_SIZE + MAX_UNIT_SIZE + 50;

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
//  float   get( String );
    float   get( prog_char* );
    String  set( String, float );
    String  getString( String );
    String  getNextSetting( String );
    void    init( prog_char*, prog_char*, float );
    void    init( prog_char*, prog_char*, prog_char*, float );
    String  getError();
  private :
    Node*  firstNode_;
    String error_;
    bool   createNode( prog_char*, prog_char*, prog_char*, float );
    void   failedNode( prog_char* );
    Node*  findNode( String );
    bool   compare( String, prog_char* );
    String nodeDescription( Node*);
};
 
#endif
