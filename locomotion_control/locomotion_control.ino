/*
  - Read x and y values from Jetson (0.0-1.0,0.0-1.0)
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */
#include <PID_v1.h> //https://playground.arduino.cc/Code/PIDLibrary/

/* Define our domains:
 *  Analog domain: [0, 1024] where [0, 512] indicates backwards and [512, 1024] is forwards. 512 means the motor should not be moving
 *  PID domain: [0.0, 1.0] which is the magnitude of the motor speed. 1.0 is the max speed. 0.0 is no speed.
 *  Input domain: [-1.0, 1.0] where a -1.0 indicates full speed backwards and 1.0 indicates full speed forwards. 0.0 is no movement
 *  PWM domain: [0, maxpwm] which is sent to the driver to indicate how fast we want motor to move. 255 is max speed
 *  
 *  We will need to convert between these domains while implementing PID control.
 */

//Define Variables we'll be connecting to
float pid_setpointL, pid_setpointR, pid_inputL, pid_inputR, pid_outputL, pid_outputR;

int dir

int pwmPinR = 11;
int cwPinR = 12;
int ccwPinR = 13;
int rpmPinR = A0;
int pwmPinL = 3;
int cwPinL = 7;
int ccwPinL = 8;
int rpmPinL = A1;
int maxpwm = 253;

float left;
float right;
float prev_left;
float prev_right;

 

void setup() {
  pinMode(pwmPinR, OUTPUT);
  pinMode(cwPinR, OUTPUT);
  pinMode(ccwPinR, OUTPUT);
  pinMode(rpmPinR, INPUT);
  pinMode(pwmPinL, OUTPUT);
  pinMode(cwPinL, OUTPUT);
  pinMode(ccwPinL, OUTPUT);
  pinMode(rpmPinL, INPUT);

//  //initialize the variables we're linked to
//  pid_inputL = analogRead(rpm);
//  pid_inputR = analogRead(0);
//  pid_setpoint = 100;
//
//  //Specify the links and initial tuning parameters
//  PID *pidR = new PID(&pid_input, &pid_output, &pid_setpoint,2,5,1, DIRECT);
//  PID *pidL = new PID(&pid_input, &pid_output, &pid_setpoint,2,5,1, DIRECT);
//
//  //turn the PID on
//  myPID.SetMode(AUTOMATIC); //turn PID on
//  myPID.SetSampleTime(200); //in ms
  
  Serial.begin(9600);
}

void loop() {
  //receive direction input of form (float,float). Ex: (1.0,1.0) means forward full speed
//  String incomingStr = "(1.0, 1.0)";
  
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    incomingStr = Serial.read();
//  }
//  
//  left = incomingStr.substring(1, incomingStr.indexOf(',')).toFloat();
//  right = incomingStr.substring(incomingStr.indexOf(',')+1, incomingStr.length()-1).toFloat();

  left = 0.5;
  right = 0.5;

  // scale setpoints to analog values
  pid_setpointL = left;
  pid_setpointR = right;

  // take in RPM input
  pid_inputL = analogRead(rpmPinL);
  pid_inputR = analogRead(rpmPinR);

  //Specify the links and initial tuning parameters
  PID *pidL = new PID(&pid_inputL, &pid_outputL, &pid_setpointL,2,5,1, DIRECT);
  PID *pidR = new PID(&pid_inputR, &pid_outputR, &pid_setpointR,2,5,1, DIRECT);



  // temporary
  prev_left = left;
  prev_right =  right;

  if (left != prev_left){
    delete pidL;
    PID *pidL = new PID(&pid_input, &pid_output, &pid_setpoint,2,5,1, DIRECT);
  }
  if (right != prev_right){
    delete pidR;
    PID *pidR = new PID(&pid_input, &pid_output, &pid_setpoint,2,5,1, DIRECT);
  }

  bool cwR = (right > 0);
  bool cwL = (left < 0);
  
  int leftpwm = left*maxpwm;
  int rightpwm = right*maxpwm;
  analogWrite(pwmPinL,leftpwm); //253 max
  analogWrite(pwmPinR,rightpwm); //253 max
  digitalWrite(cwPinR, cwR); //Direction --forward
  digitalWrite(cwPinL, cwL); //Direction
  digitalWrite(ccwPinR, ~cwR); //Direction
  digitalWrite(ccwPinL, ~cwL); //Direction
    
  pid_input = anlgToRPM(analogRead(rpmPinR));
  pidR.Compute();
  analogWrite(3,pid_output);

  pid_input = anlgToRPM(analogRead(rpmPinL));
  pidL.Compute();
  analogWrite(3,pid_output);

  prev_left = left;
  prev_right = right;

  
}

float anlgToPID(int anlg){
  anlg = anlg - 512; // shift to [-512, 512]
  int dir = anlg > 0 ? 1 : 0;
  anlg = abs(anlg); // reflect over x-axis to get positive values
  return anlg / 512.0;
  
}

int anlgToRpm(int anlg){ //2v = 0rpm, 4v = 8000rpm, 0v = -8000
  anlg = anlg-512; //[-512, 512]
  return anlg*15.625; //[-8000, 8000]
}
