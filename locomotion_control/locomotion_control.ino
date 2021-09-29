/*
  - Read x and y values from Jetson (0.0-1.0,0.0-1.0)
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */
 //#include <PID_v1.h>

int pwmPinR = 11;
int cwPinR = 12;
int ccwPinR = 13;
int rpmPinR = A0;
int pwmPinL = 3;
int cwPinL = 7;
int ccwPinL = 8;
int rpmPinL = A1;
int maxpwm = 253;
 

void setup() {
  pinMode(pwmPinR, OUTPUT);
  pinMode(cwPinR, OUTPUT);
  pinMode(ccwPinR, OUTPUT);
  pinMode(rpmPinR, INPUT);
  pinMode(pwmPinL, OUTPUT);
  pinMode(cwPinL, OUTPUT);
  pinMode(ccwPinL, OUTPUT);
  pinMode(rpmPinL, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  //receive direction input of form (float,float). Ex: (1.0,1.0) means forward full speed
  String incomingStr = "(1.0, 1.0)";
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingStr = Serial.read();
  }
  
  float left = incomingStr.substring(1, incomingStr.indexOf(',')).toFloat();
  float right = incomingStr.substring(incomingStr.indexOf(',')+1, incomingStr.length()-1).toFloat();
  bool cwR = (right > 0);
  bool cwL = (left < 0);
  
//  Serial.println(left);
//  Serial.println(right);
//  Serial.println("");
  
  int leftpwm = left*maxpwm;
  int rightpwm = right*maxpwm;
  analogWrite(pwmPinL,leftpwm); //253 max
  analogWrite(pwmPinR,rightpwm); //253 max
  digitalWrite(cwPinR, cwR); //Direction --forward
  digitalWrite(cwPinL, cwL); //Direction
  digitalWrite(ccwPinR, ~cwR); //Direction
  digitalWrite(ccwPinL, ~cwL); //Direction
  

  //Control
  // ratio L/R
//   enc1 = [10, 65000, 10] //need wrap around case
//   enc2 = [10, 21. 32]
//   enc1avg = ((enc1[1]- enc1[0]) + (enc1[2]-enc1[1])) /2 //rolling average of differences
//   enc2avg = ((enc2[1]- enc2[0]) + (enc2[2]-enc2[1])) /2 //rolling average of differences
//   //assuming want same speed
//   if (enc1avg > enc2avg){ //asume neither at edge, add a buffer later
//      //slow motor1
//   }
//   else if (enc2avg > enc1avg){
//    //slow motor2
//   }
  //idea: /\/\/\/\ - subtract to center on 0- take abs(); problem: aliasing if reads same up/down value
  
  //int avspeed = analogRead(A1);
//  Serial.println(speed);
//  //Serial.println(avspeed);
//  Serial.println("");
}

int anlgToRpm(int anlg){ //2v = 0rpm, 4v = 8000rpm, 0v = -8000
  anlg = anlg-512; //[-512, 512]
  return anlg*15.625; //[-8000, 8000]
}
