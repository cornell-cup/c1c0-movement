#include "Protocol.h"
uint8_t fsm_buffer[256];
r2pf_t fsm;
void setup() {
  fsm = r2pf_init(fsm_buffer, 256);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
char s = 'a';
void loop() {

    if(Serial1.available() > 0)
    {
      uint8_t b = Serial.read();
      r2pf_read(&fsm, b);
    }
    if (fsm.done)
    digitalWrite(13,HIGH);

}
//comment
