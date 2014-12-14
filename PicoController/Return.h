#include <Arduino.h>

const uint8_t RETURN_PIN = 0;

class Return{
  public:
    Return();
    void enable();
    void disable();
    bool getPumpStatus();
    
  private:
    bool pumpStatus_;
    void pumpOn();
    void pumpOff();
    void pumpInit();
    
};

