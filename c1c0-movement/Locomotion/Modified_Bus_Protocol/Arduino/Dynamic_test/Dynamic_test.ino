#include "Dynamic_protocol.h"

/*
Dark: address = 8
Lite: address = 4
*/
uint32_t dataLength = 10; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 3; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[14]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint16_t t;
uint32_t data_len;
uint8_t * data;
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
void printArr(uint8_t * arr,uint8_t len){
  for(int i = 0; i < len;i++){
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(38400);
  pinMode(13,OUTPUT);
  while (Serial1.available() > 0){
    Serial1.read();
  }
}
 // integer for length of data to be inserted into
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      uint32_t l1 = R2P_HEADER_SIZE - 3;
      Serial1.readBytes(recv_buffer,l1); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      r2p_dynamic_decode(recv_buffer,address,l1,&checksum,type, &data_len, &t); // decoding received data
      uint32_t l2 = data_len + 3;
      //printmsg();
      //delay(10);
      uint8_t * recv_buffer2 = (uint8_t *)malloc(sizeof(uint8_t) * l2);
      Serial1.readBytes(recv_buffer2,l2);
      buffer_len = data_len + R2P_HEADER_SIZE;
      uint8_t * recv_buffer3 = (uint8_t *)malloc(sizeof(uint8_t) * buffer_len);
      for(int i = 0; i < buffer_len;i++){
        if(i < l1)
          recv_buffer3[i] = recv_buffer[i];
        else
          recv_buffer3[i] = recv_buffer2[i - l1];
        }
             printArr(recv_buffer3,buffer_len);
      data = (uint8_t *)malloc(sizeof(uint8_t) * data_len);
      r2p_decode(recv_buffer3,address,buffer_len,&checksum,type,data, &data_len, &t); // decoding received data
      printmsg();
      free(recv_buffer2);
      free(recv_buffer3);
      uint8_t * send_buffer = (uint8_t *)malloc(sizeof(uint8_t) * (data_len + R2P_HEADER_SIZE));
      uint8_t * send_data = (uint8_t *)malloc(sizeof(uint8_t) * data_len);
      for(int i = 0; i < data_len; i++){
        data[i] = i;
      }
      uint32_t written = r2p_encode(type, address, send_data, data_len, send_buffer, 256);
      Serial1.write(send_buffer, written);
      free(send_buffer);
      free(data);
      free(send_data);
      
      }


  

      

  
      
      
      
   }
