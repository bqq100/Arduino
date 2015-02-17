#include <Arduino.h>

#define RESET_PIN          2

void initResetPin(){
  pinMode(RESET_PIN,OUTPUT); 
  digitalWrite(RESET_PIN,LOW);
}

void resetFunc(){
  delay(100);
  digitalWrite(RESET_PIN,HIGH);
}

String boolToString(bool input){
  if(input)
    return "true";
  else
    return "false";
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}




