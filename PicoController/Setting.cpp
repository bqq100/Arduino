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

float Setting::get( prog_char* name ){
  char* variableArray = new char[MAX_NAME_SIZE];
  strcpy_P(variableArray, name);
  Node* thisNode = findNode( variableArray );
  strcpy_P(variableArray, thisNode->name);
  delete( variableArray );
  if ( thisNode )
    return thisNode->value;
  else
    return -1;
}

Node* Setting::findNode( char* variable ){
  Node* searchNode = firstNode_;
  if ( strcmp_P( variable, searchNode->name ) == 0 )
    return searchNode;
  while(searchNode->next){
    searchNode = searchNode->next;
    if ( strcmp_P( variable, searchNode->name ) == 0 )
      return searchNode;
  }
  return 0;
}

void Setting::getString( char* variable ){
  Node* thisNode = findNode( variable );
  if ( thisNode )
    nodeDescription( variable, thisNode );
  else
    strcpy(variable, "Could not find setting ");  
}
  
void Setting::nodeDescription( char* variable, Node* thisNode ){
  strcpy_P( variable, thisNode->desc );
  strcat  ( variable, " (" );
  strcat_P( variable, thisNode->name );
  strcat  ( variable, ") = " );
  char value[10];
  dtostrf(thisNode->value,4,1,value);
  strcat  ( variable, value );
  if ( thisNode->unit ){
    strcat  ( variable, " " );
    strcat_P( variable, thisNode->unit );
  }
  return;
}
 
void Setting::set( char* variable, float value ){
  Node* thisNode = findNode( variable );
  if ( thisNode )
    thisNode->value = value;
  getString( variable );
}
 
void Setting::getNextSetting( char* variable ){
  Node* nextNode;
  if ( strlen(variable) == 0 )
    nextNode = firstNode_;
  else
    nextNode = findNode( variable )->next;
    
  if ( nextNode == 0 )
    *variable = (char) 0;
  else
    strcpy_P(variable, nextNode->name);

  return;
    
}
