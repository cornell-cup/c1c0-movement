#include "C:\Users\Liam Kain\Documents\Project Team\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
uint8_t fsm_buffer[256];
r2pf_t fsm;
void setup() {
  fsm = r2pf_init(fsm_buffer, 256);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
char s = 'a';
void printmsg(){
  Serial.print("Address: ") ;
  Serial.println(fsm.address);
  Serial.print("Type : ");
  Serial.println(fsm.type);
  Serial.print("Length: ") ;
  Serial.println(fsm.data_len);
  Serial.print("Data : ");
  for(int i = 0; i < fsm.data_len; i ++){
  Serial.println(i);
  Serial.println(fsm.data[i]);
  }
  Serial.println("" );
  
}
void loop() {
    if(Serial1.available() > 0)
    {
      uint8_t b = Serial1.read();
      r2pf_read(&fsm, b, 4);
      if (fsm.done == 1)
      {
        printmsg();
        
          digitalWrite(10,HIGH);
          delay(1000);
          digitalWrite(10,LOW);
          delay(1000);
        
      fsm.done = 0;
      }
   }
}
