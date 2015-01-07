#include "Setting.h"

prog_char UPDATE_FREQ_NAME[] PROGMEM = "UpdateFreq";
prog_char UPDATE_FREQ_DESC[] PROGMEM = "Auto Status Update Frequency";
prog_char UPDATE_FREQ_UNIT[] PROGMEM = "secs";

prog_char MAX_UPDATE_NAME[] PROGMEM = "MaxUpdateTime";
prog_char MAX_UPDATE_DESC[] PROGMEM = "Auto Status Max Time";
prog_char MAX_UPDATE_UNIT[] PROGMEM = "hrs";

Setting::Setting(){
  firstNode_ = 0;
  init( &UPDATE_FREQ_NAME[0] , &UPDATE_FREQ_DESC[0] , 5   );
  init( &MAX_UPDATE_NAME[0]  , &MAX_UPDATE_DESC[0]  , 168 );    
}

void Setting::init ( prog_char* name, prog_char* description, float value ){
  Node* thisNode  = new Node;
  thisNode->name = name;
  thisNode->desc = description;
  thisNode->value = value;
  thisNode->next  = 0;
  if ( firstNode_ ){
    Node* searchNode = firstNode_;
    while ( searchNode->next ){
      searchNode = searchNode->next;
    }
    searchNode->next = thisNode;
  }else{
    firstNode_ = thisNode;
  }
}

float Setting::get( String variable ){
  Node* searchNode = firstNode_;
  char variableArray[50];
  char nameArray[50];
  variable.toCharArray(variableArray, 50);
  strcpy_P(nameArray, searchNode->name);
  while ( strcmp(nameArray, variableArray ) != 0 && searchNode->next ){
    searchNode = searchNode->next;
    strcpy_P(nameArray, searchNode->name);
  }
  if ( strcmp(nameArray, variableArray) == 0 )
    return searchNode->value;
  else
    return -1;
}

String Setting::getString( String variable ){
  String message = "";
  Node* searchNode = firstNode_;

  char* variableArray = new char[50];
  char* nameArray = new char[50];
  char* descArray = new char[50];

  variable.toCharArray(variableArray, 50);
  strcpy_P(nameArray, searchNode->name);
  strcpy_P(descArray, searchNode->desc);

  while ( strcmp(nameArray, variableArray) != 0 && searchNode->next ){
    searchNode = searchNode->next;
    strcpy_P(nameArray, searchNode->name);
    strcpy_P(descArray, searchNode->desc);
  }
  if ( strcmp(nameArray, variableArray) == 0 ){
    message.concat(descArray);
    message.concat("(");
    message.concat(nameArray);
    message.concat(") = ");
    message = message + searchNode->value + "\n\r";
//    if ( searchNode->unit )
//      message = message + *searchNode->unit;
  }else{
    message = "Could not find setting " + variable + "\n\r";
  }
  return message;
}

String Setting::set( String variable, float value ){
  String message = "";
  Node* searchNode = firstNode_;

  char variableArray[50];
  char nameArray[50];
  char descArray[50];
  variable.toCharArray(variableArray, 50);
  strcpy_P(nameArray, searchNode->name);
  strcpy_P(descArray, searchNode->desc);

  while ( strcmp(variableArray, nameArray) != 0 && searchNode->next ){
    searchNode = searchNode->next;
    strcpy_P(nameArray, searchNode->name);
    strcpy_P(descArray, searchNode->desc);
  }
  if ( strcmp(nameArray, variableArray) == 0 ){
    searchNode->value = value;
    message.concat(descArray);
    message.concat("(");
    message.concat(nameArray);
    message.concat(") = ");
    message = message + searchNode->value + "\n\r";
//    if ( *searchNode->unit )
//      message = message + *searchNode->unit;
  }else{
    message = "Could not find setting " + variable + "\n\r";
  }
  return message;
}
