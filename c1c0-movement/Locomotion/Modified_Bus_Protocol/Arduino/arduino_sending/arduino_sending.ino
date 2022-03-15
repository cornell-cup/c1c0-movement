#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
int ID = 9;
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  printBuff(send_buffer,written);
  Serial1.write(send_buffer, written);
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }

  Serial.println();
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
uint8_t data[] = {0x00d,0x00b,0x00c};
uint8_t data2[] = {0x00c,0x00b,0x00d};
char s = 'a';
void loop() {
    //sending byte array "STUFF" to address 8  
   send("ON", ID, data, 8);
   delay(200);
   //sending byte array "STUFF" to address 10
   send("ON", ID, data2, 8);
   delay(200);
   
}

  // put your main code here, to run repeatedly:
