/*
  - Read x and y values from Jetson
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */

#define cw(x) ((x) <= 0 ? 0 : 1)


int pwmPinR = 3;
int dirPinR = 8;
int rpmPinR = A0;
int pwmPinL = 5;
int dirPinL = 7;
int rpmPinL = A1;
int maxrpm = 8000; //placeholder
 

void setup() {
  pinMode(pwmPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);
  pinMode(rpmPinR, INPUT);
  pinMode(pwmPinL, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(rpmPinL, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  //receive direction input
  float left = 0.5;
  float right 0.5;
  int leftrpm = left*maxrpm;
  int rightrpm = right*maxrpm;
  analogWrite(pwmPin1,leftrpm); //253 max
  analogWrite(pwmPin2,rightrpm); //253 max
  digitalWrite(dirPin1, HIGH); //Direction
  digitalWrite(dirPin2, HIGH); //Direction
  

  //Control
//  int speed1 = analogRead(rpmPin1);
//  int rpm1 = anlgToRpm(speed1);
//  int speed2 = analogRead(rpmPin2);
//  int rpm2 = anlgToRpm(speed2);
  // rpmleft, rpmright, encleft, encright
  // ratio L/R
   enc1 = [10, 65000, 10] //need wrap around case
   enc2 = [10, 21. 32]
   enc1avg = ((enc1[1]- enc1[0]) + (enc1[2]-enc1[1])) /2 //rolling average of differences
   enc2avg = ((enc2[1]- enc2[0]) + (enc2[2]-enc2[1])) /2 //rolling average of differences
   //assuming want same speed
   if (enc1avg > enc2avg){ //asume neither at edge, add a buffer later
      //slow motor1
   }
   else if (enc2avg > enc1avg){
    //slow motor2
   }
  //idea: /\/\/\/\ - subtract to center on 0- take abs(); problem: aliasing if reads same up/down value
  
  //int avspeed = analogRead(A1);
//  Serial.println(speed);
//  //Serial.println(avspeed);
//  Serial.println("");
//}

int anlgToRpm(int anlg){ //2v = 0rpm, 4v = 8000rpm, 0v = -8000
  anlg = anlg-512; //[-512, 512]
  return anlg*15.625; //[-8000, 8000]
}
