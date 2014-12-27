#ifndef Command_h
#define Command_h

#include <Arduino.h>
#include "Ato.h"
#include "Temp.h"
#include "Return.h"

class Command{
  public :
    Command();
    void check();
    Ato *ato_;
    Temp *temperature_;
    Return *returnPump_;
  private :
    String command_;
    String line_;
    void output();
    void processCommand();
};
 
#endif
