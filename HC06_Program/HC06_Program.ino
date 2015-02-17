// HC-05
// Note: Some commands require a ? at the end
// Note: To set values use AT+CMD=PARAM
// AT : Ceck the connection.
// AT+NAME : See default name
// AT+ADDR : see default address
// AT+VERSION : See version
// AT+UART : See baudrate, stop bit, parity
// AT+ROLE: See role of bt module(1=master/0=slave)
// AT+RESET : Reset and exit AT mode
// AT+ORGL : Restore factory settings
// AT+PSWD: see default password

// HC-06
// AT : check the connection
// AT+NAMExxxxxx: Change name. No space between name and command.
// AT+BAUDx: change baud rate, x is baud rate code, no space between command and code.
// AT+PINxxxx: change pin, xxxx is the pin, again, no space.
// AT+VERSION
// 1 set to 1200bps
// 2 set to 2400bps
// 3 set to 4800bps
// 4 set to 9600bps (Default)
// 5 set to 19200bps
// 6 set to 38400bps
// 7 set to 57600bps
// 8 set to 115200bps

#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX

char prev;
char c;

void setup() {

  Serial.begin(9600);

  pinMode(2,OUTPUT); 
  digitalWrite(2,LOW);

  Serial.println("Enter AT commands:");

  mySerial.begin(9600); 

}

void loop()
{

  if ( Serial.available() )
  {
    prev = c;
    c = Serial.read();
    mySerial.write(c);
  }

  if ( mySerial.available() )
    Serial.write(mySerial.read());

  if( prev == '0' && c == ' ' )
  {
    prev = (char) 0;
    c = (char) 0;
    Serial.write(0x14);  // reply two char to avrdude.exe
    Serial.write(0x10);  //  for synchronization 
    delay(100);
    digitalWrite(2,HIGH);  // turn on the relay to ground the reset pin
  }

}
