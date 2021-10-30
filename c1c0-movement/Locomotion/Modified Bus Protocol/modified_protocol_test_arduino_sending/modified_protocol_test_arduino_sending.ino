#include "C:\Users\Liam Kain\Documents\Project Team\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  Serial1.write(send_buffer, written);
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}

char s = 'a';
void loop() {
    //sending byte array "STUFF" to address 8  
   send("ON", 8, reinterpret_cast<const uint8_t*>("DUE"), 8);
   delay(2000);
   //sending byte array "STUFF" to address 10
   send("ON", 10, reinterpret_cast<const uint8_t*>("Jetson"), 8);
   delay(2000);
   
}

  // put your main code here, to run repeatedly:
