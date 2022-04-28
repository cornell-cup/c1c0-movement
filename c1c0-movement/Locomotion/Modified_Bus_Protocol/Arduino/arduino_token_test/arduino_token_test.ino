#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
//#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\R2protocol.h"

uint16_t token = 2;
uint32_t* PDR = (uint32_t*)0x400E0E04; 
const uint32_t dataLength = 1; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 2; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[R2P_HEADER_SIZE + dataLength]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint8_t data[dataLength]; //the array which data will be inserted into
uint32_t data_len; // integer for length of data to be inserted into
uint32_t send_data_len = 10;
uint8_t datalast;
uint8_t send_buffer[256];
void sendEnter(){
  pinMode(18,OUTPUT);
  *PDR |= (0x01 << 11);
}
void sendExit(){
  Serial1.flush();
  pinMode(18,INPUT);
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }
}
/* Sends data with normal R2 Protocol*/
void R2Send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len){
 // uint32_t written = r2p_encode(type, data, send_data_len, send_buffer, 256);
  //printBuff(send_buffer,written);
  //Serial1.write(send_buffer, written);
}
uint8_t senddata[10];
/* Sends data with modified protocol*/
void send2(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {

  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  //printBuff(send_buffer,written);
  Serial1.write(send_buffer, written);
  

}
void setup() {
  Serial.begin(9600);
  Serial1.begin(38400);
  pinMode(13,INPUT);
  pinMode(18,INPUT);
  for(int i = 0;i < 10;i++){
    senddata[i] = 0x04;
  }
  while(Serial1.available() > 0)
    Serial1.read();
}
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


//uint8_t senddata[] = {0x01, 0x02, 0x03};
uint8_t data2[] = {0x00c,0x00b,0x00d};
void loop() {
    uint8_t start = 0;
    if(Serial1.available() > 0 && start == 0) //checks if there is data in the serial buffer to be read
    {
      Serial1.readBytes(recv_buffer,R2P_HEADER_SIZE + dataLength); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      //Serial.println("Buffer : ");
      //printBuff(recv_buffer, R2P_HEADER_SIZE + dataLength);
      start = 1;
      int32_t x = r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len);
      //r2p_decode(recv_buffer,buffer_len,&checksum,type,data, &data_len); // decoding received data with normal protocol
      
      printmsg();
      if(dataLength == 1)
        token = data[0];

    }
    if(token == 1){
      token = 2;
      sendEnter();
    }
 
   send2("ON", address, senddata, send_data_len);
   if(token == 0){
      token = 2;
      sendExit();

    }
}
      
    
