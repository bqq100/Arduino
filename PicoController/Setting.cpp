#include "Setting.h"

Setting::Setting(){
  firstNode_ = 0;
  error_ = "";
}

String Setting::getError(){
  return error_;
}

void Setting::init( prog_char* name, prog_char* description, float value ){
  if ( createNode( name, description, 0, value ) == false )
    failedNode( name ); 
}

void Setting::init( prog_char* name, prog_char* description, prog_char* unit, float value ){
  if ( createNode( name, description, unit, value ) == false )
    failedNode( name );
}

void Setting::failedNode( prog_char* name ){
  char nameArray[MAX_NAME_SIZE];
  int i;
  for( i = 0; i < MAX_NAME_SIZE && (int)pgm_read_byte( name + 1 ) != 0; i++ )
    nameArray[i] = (char)pgm_read_byte_near( name + i );
  nameArray[i] = (char)0;  
  if ( error_.length() > 0 )
    error_ += "," + String(nameArray);
  else
    error_ = "Could initialize the following setting(s):" + String(nameArray);  
}

bool Setting::createNode( prog_char* name, prog_char* description, prog_char* unit, float value ){
  if ( strlen_P( name ) > MAX_NAME_SIZE )
    return false;
  if ( strlen_P( description ) > MAX_DESC_SIZE )
    return false;
  if ( unit && strlen_P( unit ) > MAX_UNIT_SIZE )
    return false;
    
  Node* thisNode  = new Node;
  thisNode->name  = name;
  thisNode->desc  = description;
  thisNode->unit  = unit;
  thisNode->value = value;
  
  if ( firstNode_ )
    thisNode->next = firstNode_;
  else
    thisNode->next = 0;
    
  firstNode_ = thisNode;
  
  return true;
}

/*
float Setting::get( String variable ){
  Node* thisNode = findNode( variable );
  if ( thisNode )
    return thisNode->value;
  else
    return -1;
}
*/

float Setting::get( prog_char* name ){
  char variableArray[MAX_NAME_SIZE];
  strcpy_P(variableArray, name);
  Node* thisNode = findNode( String(variableArray) );
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
  Node* thisNode = findNode( variable );
  if ( thisNode )
    return nodeDescription( thisNode );
  else
    return "Could not find setting " + variable;  
}
  
String Setting::nodeDescription( Node* thisNode ){
  String message;
  char* description = new char[MAX_SIZE];
  strcat_P( description, thisNode->desc );
  strcat  ( description, " (" );
  strcat_P( description, thisNode->name );
  strcat  ( description, ") = " );
  message = String(description) + thisNode->value;
  delete(description);
  if ( thisNode->unit ){
    char* unit = new char[MAX_UNIT_SIZE];
    strcpy_P( unit, thisNode->unit );
    message += " " + String(unit);
    delete(unit);
  }
  return message;
}
 
String Setting::set( String variable, float value ){
  Node* thisNode = findNode( variable );
  if ( thisNode )
    thisNode->value = value;
  return getString( variable );
}
  
bool Setting::compare( String variable, prog_char* progMem ){
  char variableArray[MAX_NAME_SIZE];
  variable.toCharArray(variableArray, MAX_NAME_SIZE);
  return strcmp_P(variableArray, progMem);
}

String Setting::getNextSetting( String variable ){
  Node* nextNode;
  char variableArray[MAX_NAME_SIZE];
  if ( variable.length() == 0 )
    nextNode = firstNode_;
  else
    nextNode = findNode( variable )->next;
    
  if ( nextNode == 0 )
    return "";
    
  strcpy_P(variableArray, nextNode->name);
  return String(variableArray);
    
}
