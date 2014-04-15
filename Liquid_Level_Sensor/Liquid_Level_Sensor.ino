#include <EEPROM.h>
#include <NewPing.h>
#include <QueueArray.h>

const byte HI_LIM_ADDR = 0;
const byte LO_LIM_ADDR = 2;
const byte TARGET_ADDR = 4;
const byte HEIGHT_ADDR = 6;

struct Config {
  int hiLimit; 
  int loLimit; 
  int target;
  int height;
};

Config readConfig();
bool readCommand();

NewPing ultrasonic(12,13,500);

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long interval = 1000;           // interval at which to blink (milliseconds)

uint8_t errorFlag = 0;
uint8_t pumpFlag = 0;
uint8_t lowFlag = 0;
uint8_t highFlag = 0;
uint8_t missed = 0;

uint8_t i = 0;
uint8_t j = 0;

Config config;

void setup() {
 Serial.begin(9600);
 Serial.println("Liquid Level Detector");
 pinMode(8, OUTPUT);
 digitalWrite(8, LOW); 
 delay(1000);
 digitalWrite(8, HIGH); 
 config = readConfig();
 ultrasonic.set_max_distance(config.height);
}

void loop()
{
  int sensor = ultrasonic.convert_mm(ultrasonic.ping_median(20));
  int level = config.height - sensor;
  bool flag = 0;
  
  if (level < config.height && level > 0){
    missed = 0;
  }else{
    missed++;
  }
  
  if (flag){
    if (level < config.loLimit)
      lowFlag = 1;
    else 
      lowFlag = 0;  
    if(level > config.hiLimit)
      highFlag = 1;
    else 
      highFlag = 0;
    if(level < config.target)
      pumpFlag = 1;
    else
      pumpFlag = 0;
    if (missed > 5)
      errorFlag = 1;
    else
      errorFlag = 0;
  }
  
  char line[100];  
  if (pumpFlag)
    sprintf(line,"PUMP ON");
  if (lowFlag)
    sprintf(line,"ERROR: Water too low (%d mm)",level);
  if (highFlag)
    sprintf(line,"ERROR: Water too high (%d mm)",level);
  if (errorFlag)
    sprintf(line,"ERROR: Generic Error Detected");
  if (pumpFlag || lowFlag || highFlag || errorFlag)
    Serial.println(line);

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    sprintf(line,"Value: %d Sensor1: %d Config: %d:%d:%d:%d",level,sensor,config.hiLimit,config.loLimit,config.target,config.height);
    Serial.println(line);
  }
  if (readCommand()){
    config = readConfig();
    ultrasonic.set_max_distance(config.height);
    digitalWrite(8, LOW);
    delay(1000);
    digitalWrite(8, HIGH);
  }
  delay(100);
}

Config readConfig(){
  Config thisConfig;
  thisConfig.loLimit = EEPROM.read(LO_LIM_ADDR+1)*256+EEPROM.read(LO_LIM_ADDR);  
  thisConfig.hiLimit = EEPROM.read(HI_LIM_ADDR+1)*256+EEPROM.read(HI_LIM_ADDR);    
  thisConfig.target = EEPROM.read(TARGET_ADDR+1)*256+EEPROM.read(TARGET_ADDR);  
  thisConfig.height = EEPROM.read(HEIGHT_ADDR+1)*256+EEPROM.read(HEIGHT_ADDR);  
  return thisConfig;
}

bool readCommand(){
  String input;
  bool seperator = 0;
  char command;
  if (Serial.available() >0) {
    delay(200);  //delay to allow buffer to fill
    command = Serial.read();
  }
  while (Serial.available()) {
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c == ':'){
        seperator = 1;
      }else if (c == '\n'){
        int temp = input.toInt();
        byte byte1 = (byte) temp;
        byte byte2 = (byte) (temp >> 8);
        switch (command){
          case 'L':
            EEPROM.write(LO_LIM_ADDR, byte1);
            EEPROM.write(LO_LIM_ADDR+1, byte2);
            return 1;
          case 'U':
            EEPROM.write(HI_LIM_ADDR, byte1);
            EEPROM.write(HI_LIM_ADDR+1, byte2);
            return 1; 
          case 'T':
            EEPROM.write(TARGET_ADDR, byte1);
            EEPROM.write(TARGET_ADDR+1, byte2);
            return 1;
          case 'H':
            EEPROM.write(HEIGHT_ADDR, byte1);
            EEPROM.write(HEIGHT_ADDR+1, byte2);
            return 1;
          case 'R':
            return 1;
        }
      }else if (seperator == 1){
        input += c;
      }
    }
  }
  return 0;
}


