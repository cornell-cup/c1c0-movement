#include "C:\Users\Liam Kain\Documents\Project Team\c1c0-movement\c1c0-movement\Locomotion\Modified Bus Protocol\modified_protocol.h"
/*


Dark: address = 8
Lite: address = 4
*/
const uint8_t dataLength = 2; // since data_len may be changed by decode, this ensures all assumed data lengths are specified manually
uint16_t checksum; //integer for checksum to be inserted into
uint8_t address = 4; // ID address for this microcontroller. If the message does not contain this address of 4, the message will not be processed
uint8_t recv_buffer[R2P_HEADER_SIZE + dataLength]; // this is the receiving buffer which the data will be put into, the data is 2 bytes long, so the buffer is 2 + the header size
uint32_t buffer_len = R2P_HEADER_SIZE + dataLength; 
char type[5]; //character array which the type literal will be inserted into
uint8_t data[2]; //the array which data will be inserted into
uint32_t data_len; // integer for length of data to be inserted into
uint8_t datalast;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
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
void loop() {
    if(Serial1.available() > 0) //checks if there is data in the serial buffer to be read
    {
      datalast = data[0];
      Serial1.readBytes(recv_buffer,R2P_HEADER_SIZE + dataLength); // reads the buffer data storing a buffer_len length of data in in recv_buffer
      r2p_decode(recv_buffer,address,buffer_len,&checksum,type,data, &data_len,&t   ); // decoding received data
      printmsg();
      if(data[0] == 11)
      digitalWrite(13,HIGH);
      else
      digitalWrite(13,LOW);
      
      }
   }
