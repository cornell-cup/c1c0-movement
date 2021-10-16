#include "Protocol.h"
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  //Serial.println(written,BIN);
  Serial1.write(send_buffer, written);
}
char s = 'a';
void loop() {
   send("ON", 8, reinterpret_cast<const uint8_t*>("STUFF"), 8);
   delay(2000);
}

  // put your main code here, to run repeatedly:
