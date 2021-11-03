#include "C:\Users\Liam Kain\Documents\Project Team\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
uint8_t address, uint32_t buffer_len, uint16_t* checksum, char type, uint8_t* data, uint32_t* data_len
void setup() {
  fsm = r2pf_init(fsm_buffer, 256);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
char s = 'a';
void printmsg(){
  Serial.print("Address: ") ;
  Serial.println(address);
  Serial.print("Type : ");
  Serial.println(type);
  Serial.print("Length: ") ;
  Serial.println(data_len);
  Serial.print("Data : ");
  Serial.println(int(data));
  Serial.println("" );
  
}
void loop() {
    if(Serial1.available() > 0)
    {
      uint8_t b = Serial1.read();
      r2p_decode(b,address,buffer_len,checksum,type, data,data_len );

        printmsg();
        
          digitalWrite(10,HIGH);
          delay(1000);
          digitalWrite(10,LOW);
          delay(1000);
        
      fsm.done = 0;
      }
   }
