#ifndef Command_h
#define Command_h

#include <Arduino.h>
#include "Ato.h"
#include "Temp.h"
#include "Return.h"

class Command{
  public :
    Command(Ato* ato, Temp* temperature, Return* returnPump);
    void check();
  private :
    Ato* ato_;
    Temp* temperature_;
    Return* returnPump_;
    String input_;
    unsigned long nextStatus_;
    unsigned long autoStatus_;
    int autoStatusFrequency_;
    int maxAutoStatusHours_;
    void output( String outputLine );
    void processCommand( String input );
    String status( String command, String option );
};
 
#endif
