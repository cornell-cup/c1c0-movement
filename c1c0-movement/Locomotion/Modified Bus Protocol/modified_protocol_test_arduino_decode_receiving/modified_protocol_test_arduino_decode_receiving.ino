#include "C:\Users\Liam Kain\Documents\Project Team\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
uint16_t checksum;
uint8_t address = 4; 
uint8_t recv_buffer[20];
uint32_t buffer_len = 20; 
//uint16_t* checkptr = & checksum; 
char type[5]; 
uint8_t data[3]; 
uint32_t data_len = 3;

void setup() {
  
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
  for(int i=0; i<data_len; i++){
    Serial.println(data[i]);
    }
  Serial.println("" );
  
}
void loop() {
    if(Serial1.available() > 0)
    {
      Serial1.readBytes(recv_buffer,19);
      r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len );

        printmsg();
        
          digitalWrite(10,HIGH);
          delay(1000);
          digitalWrite(10,LOW);
          delay(1000);
        

      }
   }
