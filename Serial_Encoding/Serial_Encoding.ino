#include "R2Protocol.h"
char mystr[13] = {'(' , '-' , '0' , '.' , '7' , '0' , ',' , ' ' , '0' , '.' , '8' , '0' , ')'};
char list[] = "loco";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

char send_buffer[256];
void send(const uint8_t* data, uint32_t data_len) {
  uint32_t written = r2p_encode(list, data, data_len, send_buffer, 256);
  Serial.println(written);
  //Serial.write(send_buffer, written);
  for(int i = 0; i < written; i ++){
    Serial1.write(send_buffer[i]);
  }
}
int counter = 0;
void loop() {
  send(mystr, 13);
  if  (counter > 10){
    Serial.println("Changing string");
    mystr[4] = '5';  
  }
  //Serial.write(mystr,5); //Write the serial data
  delay(1000);
  counter++;
}
