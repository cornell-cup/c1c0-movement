#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
/*
Dark: address = 8
Lite: address = 4
*/
const uint32_t dataLength = 10; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 3; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[R2P_HEADER_SIZE + dataLength]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint8_t data[2]; //the array which data will be inserted into
uint32_t data_len; // integer for length of data to be inserted into
uint8_t datalast;
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  //Serial.println("sending stuff");
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  Serial1.write(send_buffer, written);
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(38400);
  pinMode(13,OUTPUT);
}
uint16_t t;
char s = 'a';
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
uint8_t senddata[] = {0x05, 0x06, 0x07};
uint8_t data2[] = {0x00c,0x00b,0x00d};
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      Serial1.readBytes(recv_buffer,R2P_HEADER_SIZE + dataLength); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      //Serial.println("Buffer : ");
      /*
      for(int i=0; i<R2P_HEADER_SIZE + dataLength; i++){
        Serial.print(recv_buffer[i]);
        Serial.print("  ");
        Serial.print(i);
      }
      */
      r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len, &t); // decoding received data
      }
      //printmsg();
      if(data[0] == 1 && data[1] == 2 && data[2] == 3){
          send("ON", address, senddata, 3);
        memcpy(type,"llll", 4);
      }
      
      
   }