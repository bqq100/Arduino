#include <Arduino.h>

String boolToString(bool input){
  if(input)
    return "true";
  else
    return "false";
}
