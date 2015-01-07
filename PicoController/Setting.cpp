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
  Node* thisNode = new Node;
  thisNode->name = name;
  thisNode->desc = description;
  thisNode->value = value;
  thisNode->next = 0;
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
  Node* thisNode = findNode( variable )
  if ( thisNode )
    return thisNode->value;
  else
    return -1;
}

Node* Setting::findNode( String variable ){
  Node* searchNode = firstNode_;
  while ( compare(variable, searchNode->name ) != 0 && searchNode->next )
    searchNode = searchNode->next;
  if ( compare(variable, searchNode->name ) == 0 )
    return searchNode;
  else
    return 0;
}

String Setting::getString( String variable ){
  Node* thisNode = findNode( variable )
  if ( thisNode )
    return nodeDescription( thisNode );
  else
    return F("Could not find setting ") + variable;  
}
  
String Setting::nodeDescription( Node* thisNode ){
  String message;
  char* description = new char[200];
  strcat_P( description, thisNode->desc );
  strcat  ( description, F(" (") );
  strcat_P( description, thisNode->name );
  strcat  ( description, F(") = ") );
  message = String(description) + searchNode->value;
      
  // if ( searchNode->unit )
  // message = message + *searchNode->unit;
  return message;
}
 
String Setting::set( String variable, float value ){
  Node* thisNode = findNode( variable )
  if ( thisNode )
    thisNode->value = value;
  return getString( variable );
}
  
bool Setting::compare( String variable, prog_char* progMem ){
  char variableArray[variable.length()+1];
  variable.toCharArray(variableArray, variable.length()+1);
  return strcmp_P(variableArray, progMemArray);
}

