/*
  - Read x and y values from Jetson (0.0-1.0,0.0-1.0)
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */
#include <PID_v1.h> //https://playground.arduino.cc/Code/PIDLibrary/
//#include "R2Protocol.h"

/* Define our domains:
 *  Analog domain: [0, 1024] where [0, 512] indicates backwards and [512, 1024] is forwards. 512 means the motor should not be moving
 *  PID domain: [0.0, 1.0] which is the magnitude of the motor speed. 1.0 is the max speed. 0.0 is no speed.
 *  Input from path planning domain: [-1.0, 1.0] where a -1.0 indicates full speed backwards and 1.0 indicates full speed forwards. 0.0 is no movement
 *  PWM domain: [0, maxpwm] which is sent to the driver to indicate how fast we want motor to move. 255 is max speed
 *  
 *  We will need to convert between these domains while implementing PID control.
 */

/*     (-,-)     
    |__ | | __|  
       |   |     
     <_     _>   
*/

// domain change functions
#define analog_to_PID(anlg) abs(((anlg/512) - 1)) 
#define PID_to_pwm(PID) (PID*max_pwm)

// pin defintions
int pwm_pin_R = 11;
int pwm_pin_L = 3;
int cw_pin_R = 12;
int ccw_pin_R = 13;
int rpm_pin_R = A0;
int cw_pin_L = 8;
int ccw_pin_L = 7;
int rpm_pin_L = A1;
int max_pwm = 253;

// in PID domain
double pid_setpoint_L, pid_setpoint_R, pid_input_L, pid_input_R, pid_output_L, pid_output_R;

// in input domain
double left, right, prev_left, prev_right;

// clockwise right and left
bool cw_R, cw_L;

// left and right pwm values to write to driver
int left_pwm;
int right_pwm;

// input string to receive from Jetson
String input_str;

PID *pid_R;
PID *pid_L;
 

void setup() {
  pinMode(pwm_pin_R, OUTPUT);
  pinMode(cw_pin_R, OUTPUT);
  pinMode(ccw_pin_R, OUTPUT);
  pinMode(rpm_pin_R, INPUT);
  pinMode(pwm_pin_L, OUTPUT);
  pinMode(cw_pin_L, OUTPUT);
  pinMode(ccw_pin_L, OUTPUT);
  pinMode(rpm_pin_L, INPUT);
  
  // get current motor movement
  pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
  pid_input_R = analog_to_PID(analogRead(rpm_pin_R));

  // init setpoint as not moving
  pid_setpoint_L = 0.0;
  pid_setpoint_R = 0.0;
//  pid_setpoint_L = 0.2;
//  pid_setpoint_R = 0.2;

  // create initial PID controllers - input, output, setpoint, tuning parameters
  pid_R = new PID(&pid_input_R, &pid_output_R, &pid_setpoint_R, 2, 5, 1, DIRECT);
  pid_L = new PID(&pid_input_L, &pid_output_L, &pid_setpoint_L, 2, 5, 1, DIRECT);

  // turn PID on and set sample time in ms
  pid_R->SetMode(AUTOMATIC);
  pid_R->SetSampleTime(200);
  pid_R->SetOutputLimits(-0.01,0.01);
  pid_L->SetMode(AUTOMATIC);
  pid_L->SetSampleTime(200);
  pid_L->SetOutputLimits(-0.01,0.01);


  // init as not moving
  left = 0.0;
  right = 0.0;
  prev_left = 0.0;
  prev_right =  0.0;
    
  // start serial
  Serial.begin(9600); 
}

void loop() {

  // read the incoming byte
//  if (Serial.available() > 0) {
//    input_str = Serial.read();
   
//    // parse input string
//    left = input_str.substring(1, input_str.indexOf(',')).toFloat();
//    right = input_str.substring(input_str.indexOf(',')+1, input_str.length()-1).toFloat();
//  }
  left = 0.2;
  right = 0.2;

  // set directions
  cw_R = (right > 0);
  cw_L = (left < 0);

  // scale setpoints to PID domain
  pid_setpoint_L = abs(left);
  pid_setpoint_R = abs(right);

  // take in RPM input from driver - analog to PID
  pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
  pid_input_R = analog_to_PID(analogRead(rpm_pin_R));
//  pid_input_L = 0.2;
//  pid_input_R = 0.2;
  
  // write pwm to drivers
  left_pwm = PID_to_pwm(pid_output_L+pid_setpoint_L);
  right_pwm = PID_to_pwm(pid_output_R+pid_setpoint_R);

  
//  Serial.println(pid_input_L);
//  Serial.println(pid_output_L);
//  Serial.println("Left PWM: " + left_pwm);
//  Serial.println("Right PWM: " + right_pwm);


  analogWrite(pwm_pin_L,left_pwm); //253 max
  analogWrite(pwm_pin_R,right_pwm); //253 max

  // write direction pins on drivers
  digitalWrite(cw_pin_R, cw_R); //Direction --forward
  digitalWrite(cw_pin_L, cw_L); //Direction
  digitalWrite(ccw_pin_R, ~cw_R); //Direction
  digitalWrite(ccw_pin_L, ~cw_L); //Direction

  // save input motor speeds
  prev_left = left;
  prev_right = right;

  // must call compute pid every loop - will only actually run every SetSampleTime ms
  pid_R->Compute();
  pid_L->Compute();
}


// // === Domain Conversion Functions ===

// float anlgToPID(int anlg){
//   anlg = anlg - 512; // shift to [-512, 512]
//   int dir = anlg > 0 ? 1 : 0;
//   anlg = abs(anlg); // reflect over x-axis to get positive values
//   return anlg / 512.0; 
// }

// int pidToPWM(float pid_value){
//   return int(pid_value * max_pwm);
// }

// int anlgToRpm(int anlg){ //2v = 0rpm, 4v = 8000rpm, 0v = -8000
//   anlg = anlg-512; //[-512, 512]
//   return anlg*15.625; //[-8000, 8000]
// }
