/*
  - Read x and y values from Jetson (0.0-1.0,0.0-1.0)
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */
#include <PID_v1.h> //https://playground.arduino.cc/Code/PIDLibrary/
#include "R2Protocol.h"

/* Define our domains:
 *  Analog domain: [0, 818] where [0, 408] indicates backwards and [410, 819] is forwards. 409 means the motor should not be moving. 
 *    This is a limitation of the driver software. The analog output only goes up to 4V. This means when it outputs 4V it is saying the motor speed is the max 
 *    rpm (8000rpm). When it outputs 2V, the motor is not moving. When it outputs 0V, the motor is moving -8000rpm. Thus, when we read 4V on the analog input pin, 
 *    it shows up as 1023*(4/5) because reading 5V on an analog pin returns 1023.
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
#define analog_to_PID(anlg) abs(((anlg/410) - 1)) 
#define PID_to_pwm(PID) (PID*max_pwm)

// r2protocol declarations;
uint8_t recv_buffer[29];
uint8_t type[5];
uint16_t checksum;
char data[29];
int32_t x = 1000;
int i = 0;
uint32_t data_len = 13;

int counter = 0;

// pin defintions
int pwm_pin_R = 11;
int pwm_pin_L = 3;
int cw_pin_R = 12;
int ccw_pin_R = 13;
int rpm_pin_R = A0;
int cw_pin_L = 8;
int ccw_pin_L = 7;
int rpm_pin_L = A1;
int max_pwm = 253; // 255 caused the motor drivers to stall, so we set max as 253

// in PID domain
double pid_setpoint_L, pid_setpoint_R, pid_input_L, pid_input_R, pid_output_L, pid_output_R;

// in input domain
float left, right;

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
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, LOW);
  
  // get current motor movement
  pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
  pid_input_R = analog_to_PID(analogRead(rpm_pin_R));

  // init setpoint as not moving
  pid_setpoint_L = 0.0;
  pid_setpoint_R = 0.0;

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
  left = 0.01;
  right = 0.01;

  counter = 0;
    
  // start serial
  Serial.begin(115200); 
  Serial2.begin(115200); 

  while(Serial2.available() > 0){
    Serial2.read();  
  }
  delay(100);
}

uint8_t num [5];

void loop() {

  // read the incoming byte
 if (Serial2.available() > 0) {
    Serial2.readBytes(recv_buffer, 29);

     x = r2p_decode(recv_buffer, 29, &checksum, type, data, &data_len);
      //data buffer of form: {'(' , '-' , '0' , '.' , '7' , '0' , ',' , '+' , '0' , '.' , '8' , '0' , ')'} 
     num[0] = data[1];
     num[1] = data[2];
     num[2] = data[3];
     num[3] = data[4];
     num[4] = data[5];
     float temp_left = atof(num);
     if(abs(temp_left) <= 1) left = temp_left; // only update value if it is valid (between -1 and 1)

     num[0] = data[7];
     num[1] = data[8];
     num[2] = data[9];
     num[3] = data[10];
     num[4] = data[11];
     float temp_right = atof(num);
     if(abs(temp_right) <= 1) right = temp_right; // only update value if it is valid (between -1 and 1)
         
//     Serial.println("Start Transaction");
//     for (int i=0; i<29; i++){
//      Serial.println(recv_buffer[i]);
//      }
//      Serial.println("");
//     for (int i=0; i<13; i++){
//      Serial.println(data[i]);
//      }

 }

//  if (counter < 500){
//    left = 0.2; // comment these out once we are receiving left and right values from the Jetson
//    right = 0.2;
//    Serial.println("                    First");
//  }
//  else if (counter >= 500 && counter <= 1000){
//    left = 0.15; // comment these out once we are receiving left and right values from the Jetson
//    right = -0.15;
//    Serial.println("                                    Second");
//  }
//  else {
//    left = -0.15; // comment these out once we are receiving left and right values from the Jetson
//    right = 0.15;
//    Serial.println("                                                          Third");
//  }

//  left = 0.2; // comment these out once we are receiving left and right values from the Jetson
//  right = 0.2;

//  Serial.print("Right: ");
//  Serial.println(right);
//  Serial.print("Left: ");
//  Serial.println(left);
////  digitalWrite(LED_BUILTIN, LOW);


//  if(left == 0.25){
//    digitalWrite(LED_BUILTIN, HIGH);
//    }
//   else{
//    digitalWrite(LED_BUILTIN, LOW);
//    }

  // set directions
  cw_R = (right > 0);
  cw_L = (left < 0);

  // scale setpoints to PID domain
  pid_setpoint_L = abs(left);
  pid_setpoint_R = abs(right);

  // take in RPM input from driver - analog to PID
  pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
  pid_input_R = analog_to_PID(analogRead(rpm_pin_R));
  
  // write pwm to drivers with PID control enabled
  left_pwm = PID_to_pwm(pid_output_L+pid_setpoint_L);
  right_pwm = PID_to_pwm(pid_output_R+pid_setpoint_R);
  // comment bottom two lines in if we dont want to use our PID
//  left_pwm = PID_to_pwm(pid_setpoint_L);
//  right_pwm = PID_to_pwm(pid_setpoint_R);

//  Serial.print("Left PWM: ");
//  Serial.println(left_pwm);
//  Serial.print("Right PWM: ");
//  Serial.println(right_pwm);

  analogWrite(pwm_pin_L,left_pwm); //253 max
  analogWrite(pwm_pin_R,right_pwm); //253 max

  // write direction pins on drivers
  digitalWrite(cw_pin_R, cw_R); //Direction --forward
  digitalWrite(cw_pin_L, cw_L); //Direction
  digitalWrite(ccw_pin_R, !cw_R); //Direction
  digitalWrite(ccw_pin_L, !cw_L); //Direction

//  Serial.print("cw_R pin: ");
//  Serial.println(digitalRead(cw_pin_R));
//  Serial.print("ccw_R pin: ");
//  Serial.println(digitalRead(ccw_pin_R));
//  Serial.print("cw_L pin: ");
//  Serial.println(digitalRead(cw_pin_L));
//  Serial.print("ccw_L pin: ");
//  Serial.println(digitalRead(ccw_pin_L));
//  Serial.print("cw_R pin: ");
//  Serial.println(cw_R);
//  Serial.print("ccw_R pin: ");
//  Serial.println(!cw_R);
//  Serial.print("cw_L pin: ");
//  Serial.println(cw_L);
//  Serial.print("ccw_L pin: ");
//  Serial.println(!cw_L);

  // must call compute pid every loop - will only actually run every SetSampleTime ms
  pid_R->Compute();
  pid_L->Compute();

  counter++;
}
