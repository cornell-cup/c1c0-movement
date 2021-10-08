#include "Protocol.h"
uint8_t fsm_buffer[256];
r2pf_t fsm;
void setup() {
  fsm = r2pf_init(fsm_buffer, 256);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
  
}
uint8_t send_buffer[256];
void send(char type[5], const uint8_t* data, uint32_t data_len) {
  uint32_t written = r2p_encode(type, data, data_len, send_buffer, 256);
  Serial.write(send_buffer, written);
}
char s = 'a';
void loop() {
  send("ON\0", reinterpret_cast<const uint8_t*>("HEADLOCO"), 8);
   }
   delay(2000);
}

  // put your main code here, to run repeatedly:
