#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
//#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\R2protocol.h"

/*
Dark: address = 8
Lite: address = 4
*/
/*.022*/

const uint32_t dataLength = 10; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 3; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[R2P_HEADER_SIZE + dataLength]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint8_t data[dataLength]; //the array which data will be inserted into
uint32_t data_len; // integer for length of data to be inserted into
uint32_t send_data_len = 3;
uint8_t datalast;
uint8_t send_buffer[256];

void printBuff(uint8_t* arr, size_t len){
  for(int i = 0; i < len; i++){
    Serial.print(arr[i]);
    Serial.print(" ");
    Serial.println();
  }
}
/* Sends data with normal R2 Protocol*/
void R2Send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len){
 // uint32_t written = r2p_encode(type, data, send_data_len, send_buffer, 256);
  //printBuff(send_buffer,written);
  //Serial1.write(send_buffer, written);
}
/* Sends data with modified protocol*/
void send2(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  pinMode(18,OUTPUT);
  REG_PIOA_PDR |= (0x01 << 11);
  //delay(100);
  //Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  //printBuff(send_buffer,written);
  Serial1.write(send_buffer, written);
  delay(3);
  pinMode(18,INPUT);
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(13,INPUT);
  pinMode(18,INPUT);
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
uint8_t senddata[] = {0x01, 0x02, 0x03};
uint8_t data2[] = {0x00c,0x00b,0x00d};
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      Serial1.readBytes(recv_buffer,R2P_HEADER_SIZE + dataLength); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      Serial.println("Buffer : ");
      
      /*for(int i=0; i<R2P_HEADER_SIZE + dataLength; i++){
        Serial.print(recv_buffer[i]);
        Serial.print("  ");
        Serial.print(i);
      }*/
      
      r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len, &t); // decoding received data with modified protocol
      //r2p_decode(recv_buffer,buffer_len,&checksum,type,data, &data_len); // decoding received data with normal protocol
      }
      //printmsg();
 
      if(strncmp(type,"rqst",4)==0){
        if(data[0] == 8){
          send2("OFF", address, senddata, 3);   
        }

        else if(data[0] == 9){
          //Serial.println("Ready");
          send2("ON", address, senddata, 3);
          //Serial.println("Done sending");
          memcpy(type,"llll", 4);
        }
    

      }
      
      
   }
