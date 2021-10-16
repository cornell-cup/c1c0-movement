#include "R2Protocol.h"
//char mystr[10]; //Initialized variable to store recieved data
uint8_t recv_buffer[21];
uint8_t type[5];
uint16_t checksum;
uint8_t data[21];
int32_t x = 1000;
int i = 0;
uint32_t data_len = 5;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial3.begin(9600);
  Serial.begin(9600);
}

void loop() {
  
  if (Serial3.available() > 0){
//    Serial.println("hi");
    Serial3.readBytes(recv_buffer, 21); //Read the serial data and store in var
//    Serial.println((unsigned long)&recv_buffer[0]);
    x = r2p_decode(recv_buffer, 21, &checksum, type, data, &data_len);
    //Serial.println("n = " + x);
//    Serial.println((unsigned long)&recv_buffer[0]);
    for (i=0; i<data_len; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(data[i]);
   }
   Serial.println("---------------------------------------------------");
//   Serial.println(x);


    
   // Serial.println(mystr); //Print data on Serial Monitor
    delay(1000);
  }
}
