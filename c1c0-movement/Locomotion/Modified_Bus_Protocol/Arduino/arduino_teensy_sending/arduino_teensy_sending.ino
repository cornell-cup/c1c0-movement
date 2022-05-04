#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"
#define TX 17

uint32_t base = 0x40184000;
//uint32_t *base = 4;
uint32_t off1  = 0xC000;
uint32_t off2 = 0x18;
uint32_t* Tenable = (uint32_t*)(base + off1);
//uint32_t* Tenable = (uint32_t*)4;

int ID = 4;
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  pinMode(17,OUTPUT);
  //delay(1000);
  //*Tenable = *Tenable | (1<<19);
  Serial4.begin(38400);
  delay(10);
  *Tenable = *Tenable | (1<<19);
   Serial.print("Reg value = ");
   Serial.println(*Tenable);
  *Tenable &= ~(1<<19);
  Serial.println("Reg value = " +  (String)*Tenable);
  delay(10);
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  printBuff(send_buffer,written);
  Serial4.write(send_buffer, written);
  
  //Serial.println("Bytes Written: " + String(written));
//  delay(30);
  Serial4.flush();
  pinMode(17,INPUT);
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }

  Serial.println();
}
void setup() {
  Serial.begin(9600);
  Serial4.begin(38400);

  //*Tenable &= ~(0x1<<1);
  //*Tenable |= (0x1<<18);
  //*Tenable |= (0x1<<18);
  pinMode(13,OUTPUT);
  
  pinMode(17,INPUT);
  while(Serial6.available() > 0)
    Serial4.read();
  //GPIO_GDIR |= (1<<17);
}
uint8_t data[] = {0x0d,0x0b,0x0c};
uint8_t data2[] = {0x0c,0x0b,0x0d};
char s = 'a';
void loop() {
    Serial.println(int(Tenable),HEX);
    //sending byte array "STUFF" to address 8
   send("ON", ID, data, 3);
   delay(200);
   //REG_PIOA_PDR |= (0x01 << 11);
   //sending byte array "STUFF" to address 10
   send("ON", ID, data2, 3);
   delay(200);
   
}

  // put your main code here, to run repeatedly:
