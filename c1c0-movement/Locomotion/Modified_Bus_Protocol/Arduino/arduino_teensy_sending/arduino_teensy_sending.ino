#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\Modified_Bus_Protocol\modified_protocol.h"

int ID = 4;
uint8_t send_buffer[256];
void send(char type[5], uint8_t address, const uint8_t* data, uint32_t data_len) {
  pinMode(24,OUTPUT);
  //delay(1000);
  Serial6.begin(38400);
  
    
  //delay(10);
  Serial.println(int(address));
  uint32_t written = r2p_encode(type, address, data, data_len, send_buffer, 256);
  printBuff(send_buffer,written);
  Serial6.write(send_buffer, written);

  //Serial.println("Bytes Written: " + String(written));
//  delay(30);
  Serial6.flush();
  pinMode(24,INPUT);
}
void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);    
  }

  Serial.println();
}
void setup() {
  Serial.begin(9600);
  Serial6.begin(115200);
  pinMode(13,OUTPUT);
  
  pinMode(24,INPUT);
  while(Serial6.available() > 0)
    Serial6.read();
  //GPIO_GDIR |= (1<<17);
}
uint8_t data[] = {0x0d,0x0b,0x0c};
uint8_t data2[] = {0x0c,0x0b,0x0d};
char s = 'a';
void loop() {
    //sending byte array "STUFF" to address 8
   send("ON", ID, data, 3);
   delay(200);
   //REG_PIOA_PDR |= (0x01 << 11);
   //sending byte array "STUFF" to address 10
   send("ON", ID, data2, 3);
   delay(200);
   
}

  // put your main code here, to run repeatedly:
