//#include "Protocol.h"

void setup() {
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
}
char s = 'a';
void loop() {

    if(Serial1.available() > 0)
    {
          s =Serial1.read();
    }
    if(s > 3)
    digitalWrite(13,HIGH);
    else
    digitalWrite(13,LOW);
}
