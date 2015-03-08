#ifndef Equip_h
#define Equip_h

#include "Module.h"

class Equip: public Module{
  public:
    Equip( Setting*, Clock*, uint8_t );
    bool getEquipStatus();
    void equipOn();
    void equipOff();
    
  protected:
    unsigned long equipOnTime_;
    bool equipStatus_;
    int  equipPin_;

//    void equipOn();
//    void equipOff();
    void equipInit();
};

#endif
