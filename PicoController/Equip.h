#ifndef Equip_h
#define Equip_h

#include "Module.h"

class Equip: public Module{
  public:
    Equip( Setting*, uint8_t );
    bool getEquipStatus();
    
  protected:
    bool equipStatus_;
    int  equipPin_;

    void equipOn();
    void equipOff();
    void equipInit();
};

#endif
