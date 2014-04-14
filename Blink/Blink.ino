/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */


int readCommand();
int target = 0;
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
 Serial.begin(9600);
 Serial.println("Testing Makefile...");
}

void loop() {
  char line[50];
  digitalWrite(13, HIGH);   // set the LED on
  Serial.println("LED On");
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  Serial.println("LED Off");
  delay(1000);              // wait for a second
  sprintf(line, "Target: %d", target);
  Serial.println(line);
  int temp;
  temp = readCommand();
  if (temp > 0){
    target = temp; 
  }
//  while (Serial.available()) {
//    delay(1000);  //delay to allow buffer to fill 
//    if (Serial.available() >0) {
//      char c = Serial.read();  //gets one byte from serial buffer
//      if (c == '\n'){
//        lastString = readString;
//        readString = "";
//      }else{
//        readString += c; //makes the string readString 
//      }
//    } 
//  }
}

int readCommand(){
  String target;
  bool seperator;
  int command;
  while (Serial.available()) {
    delay(1000);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c == 'S'){
        command = 1;
      }else if(c == ':'){
        seperator = 1;
      }else if (c == '\n'){
        return target.toInt();
      }else if (command == 1 && seperator == 1){
        target += c;
      }
    }
  }
  return 0;
}
