#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
//make token system
int ID = 9;
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  digitalWrite(13,HIGH);
  //delay(100);
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  printBuff(send_buffer,written);
  Serial1.write(send_buffer, written);
  delay(10);
  digitalWrite(13,LOW);
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }

  Serial.println();
}
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(13,OUTPUT);
  //digitalWrite(13,HIGH);
}
uint8_t data[] = {0x0d,0x0b,0x0c};
uint8_t data2[] = {0x0c,0x0b,0x0d};
char s = 'a';
void loop() {
    //sending byte array "STUFF" to address 8 
   send("ON", ID, data, 3);
   delay(200);
   //sending byte array "STUFF" to address 10
   send("ON", ID, data2, 3);
   delay(200);
   
}

  // put your main code here, to run repeatedly:
