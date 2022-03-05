//#include "C:\Users\Liam Kain\Documents\ProjectTeam\c1c0-movement\c1c0-movement\Locomotion\R2Protocol.h";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);

  while (Serial1.available() > 0){
    Serial1.read();
  }
  delay(100);
}

void printBuff(uint8_t buf[], int len){
  for(int i = 0; i < len; i++){
    Serial.print(buf[i],HEX);
    Serial.print(" ");    
  }
  Serial.println();
}
uint8_t buf[10];
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      Serial1.readBytes(buf,10); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      printBuff(buf,10);
      delay(100);
      }
   }
