#include <Arduino.h>

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
