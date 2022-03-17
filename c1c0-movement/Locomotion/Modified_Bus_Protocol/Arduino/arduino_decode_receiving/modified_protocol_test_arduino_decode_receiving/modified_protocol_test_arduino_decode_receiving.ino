#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
//#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\R2Protocol.h";

const uint8_t dataLength = 3; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 3; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[R2P_HEADER_SIZE + dataLength]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint8_t data[3]; //the array which data will be inserted into
uint32_t data_len; // integer for length of data to be inserted into
uint8_t datalast;
int i = 0;
void setup() {
  Serial.begin(9600);
  Serial1.begin(38400);
  pinMode(13,OUTPUT);

  while (Serial1.available() > 0){
    Serial1.read();
  }
  delay(100);
}
uint16_t t;
void printmsg(){
  Serial.print("Address: ") ;
  Serial.println(address);
  Serial.print("Type : ");
  Serial.println(type);
  Serial.print("Length: ") ;
  Serial.println(data_len);
  Serial.println("Data : ");
   for(int i=0; i<data_len; i++){
     Serial.print(data[i]);
     Serial.print("  ");
     }
   Serial.println(" ");

  
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }

  Serial.println();
}
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      
      datalast = data[0];
      
      Serial1.readBytes(recv_buffer,R2P_HEADER_SIZE + dataLength); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      printBuff(recv_buffer,R2P_HEADER_SIZE + dataLength);
      r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len, &t); // decoding received data
      //r2p_decode(recv_buffer,buffer_len,&checksum,type,data, &data_len);
      printmsg();
      if(datalast != data[0]){
         if(data[0] == 11)
           digitalWrite(13,HIGH);
         else
           digitalWrite(13,LOW);
      }

      }

   }
