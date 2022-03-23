#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
//make token system
int ID = 9;
volatile uint8_t* CR = (uint8_t*)0x4009C000;
uint8_t* PIODDis = (uint8_t*)0x400E1204;
uint8_t* PIODEn = (uint8_t*)0x400E1400;
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
void send2(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  pinMode(18,OUTPUT);
  REG_PIOA_PDR |= (0x01 << 11);
  //delay(100);
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  printBuff(send_buffer,written);
  Serial1.write(send_buffer, written);
  delay(10);
  pinMode(18,INPUT);
}
void T2_Enable(){
  *PIODEn |= (1<<18);
}
void T2_Disable(){
  *PIODDis |= (1<<18);
}
void T_Enable(){
  *CR = *CR | 64;
}
void T_Disable(){
  *CR = *CR | (1<<7);
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
  pinMode(18,INPUT);
  //T2_Disable();
  //T_Disable();
}
uint8_t data[] = {0x0d,0x0b,0x0c};
uint8_t data2[] = {0x0c,0x0b,0x0d};
char s = 'a';
void loop() {
    //sending byte array "STUFF" to address 8 
   //Serial1.begin(115200);
   //send2("ON", ID, data, 3);
   delay(200);
   //T2_Enable();
   //sending byte array "STUFF" to address 10
   //Serial1.setTX(12);
   //send2("ON", ID, data2, 3);
   //Serial.println(getPinMode(18));
   delay(200);
   
}
