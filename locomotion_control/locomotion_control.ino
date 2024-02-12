/*
  - Read x and y values from Jetson (±0.0-1.0,±0.0-1.0)
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback (PID control)

 */
#include <PID_v1.h> //https://playground.arduino.cc/Code/PIDLibrary/
#include "R2Protocol.h"
#include <Servo.h>

/*  Define our domains:
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
        | | ==|  <3
       |   |
       _>  _>
*/

// domain change functions
#define analog_to_PID(anlg) abs(((anlg / 410) - 1))
#define PID_to_pwm(PID) (PID * max_pwm)

// r2protocol declarations;
uint8_t recv_buffer[29];
uint8_t type[5];
uint16_t checksum;
char data[29];
int32_t x = 1000;
int i = 0;
uint32_t data_len = 13;

// r2protocol - arduino to jetson
char msg[4] = "ack";
uint8_t msg_data_buffer[1];
uint8_t msg_send_buffer[2048];

Servo headServo; // create servo object to control a servo, currently set up for the HS-755HB servo (non-continuous rotation between 0 and 202 degrees
bool absolute;   // variable to represent when the angle taken from the serial port is an absolute angle or a change in angle (1 if absolute)
bool negative;   // variable to represent when a change in angle is negative (1 is negative)
int turnspeed;
bool head = false;
int headdata[3];

int counter = 0;

bool zero_flagL;
bool zero_flagR;

// pin defintions
int pwm_pin_R = 11;
int pwm_pin_L = 3;
int cw_pin_R = 12;
int ccw_pin_R = 13;
int rpm_pin_R = A0;
int cw_pin_L = 4;
int ccw_pin_L = 5;
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

void setup()
{
  pinMode(pwm_pin_R, OUTPUT);
  pinMode(cw_pin_R, OUTPUT);
  pinMode(ccw_pin_R, OUTPUT);
  pinMode(rpm_pin_R, INPUT);
  pinMode(cw_pin_L, OUTPUT);
  pinMode(ccw_pin_L, OUTPUT);
  pinMode(rpm_pin_L, INPUT);
  analogWrite(pwm_pin_L, 80 );  // 253 max
  pinMode(10, OUTPUT);


  // get current motor movement
  pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
  pid_input_R = analog_to_PID(analogRead(rpm_pin_R));

  // init setpoint as not moving
  pid_setpoint_L = 0.2;
  pid_setpoint_R = 0.2;

  // create initial PID controllers - input, output, setpoint, tuning parameters
  pid_R = new PID(&pid_input_R, &pid_output_R, &pid_setpoint_R, 2, 0, 1, DIRECT);
  pid_L = new PID(&pid_input_L, &pid_output_L, &pid_setpoint_L, 2, 0, 1, DIRECT);

  // turn PID on and set sample time in ms
  pid_R->SetMode(AUTOMATIC);
  pid_R->SetSampleTime(200);
  pid_R->SetOutputLimits(-0.01, 0.01);
  pid_L->SetMode(AUTOMATIC);
  pid_L->SetSampleTime(200);
  pid_L->SetOutputLimits(-0.01, 0.01);

  // init as not moving
  left = 0.2;
  right = 0.2;
  zero_flagL = 1;
  zero_flagR = 1;

  counter = 0;

  // pins 5 and 6 are BAD!!!! Do not use
  headServo.attach(10, 556, 2410); // attaches the servo on pin 10 to the servo object, PWM range between 556-2410 for the HS-755HB (change for different servos)
                                   //    headServo.attach(10);

  // start serial
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("setup");
  
  while (Serial1.available() > 0) {
    Serial1.read();
  }

  // acknowledgement message
  for(int i = 0; i < 3; i++) {
    msg_data_buffer[i] = (uint8_t) msg[i];
  }
  delay(100);
}

uint8_t num[5];

void loop()
{
  send("LOCR", msg_data_buffer, 3, msg_send_buffer);
  delay(250);
  // read the incoming byte
  //  headServo.write(110);
  if (Serial1.available() > 0)
  {
    Serial1.readBytes(recv_buffer, 29);
    if(r2p_decode(recv_buffer, 29, &checksum, type, data, &data_len) > 0){
      // send back some acknowledgement message here
      send("LOCR", msg_data_buffer, 3, msg_send_buffer);
      // data buffer of form: {'(' , '-' , '0' , '.' , '7' , '0' , ',' , '+' , '0' , '.' , '8' , '0' , ')'}

      Serial.print("Checksum: ");
      Serial.println(checksum);

      if (data[0] == 'h' && data[1] == 'e' && data[2] == 'a' && data[3] == 'd')
      {
        head = true;
      }
      else
        head = false;

      Serial.print("Head?: ");
      Serial.println(head);
      // ================================ Head Servo ================================

      if (head)
      {
        headdata[0] = int(data[10]) - 48;
        headdata[1] = int(data[11]) - 48;
        headdata[2] = int(data[12]) - 48;
        for (int i = 0; i < 3; i++)
        {
          Serial.println(headdata[i]);
        }
        absolute = headdata[1];
        Serial.println(headdata[2]);
        if (headdata[2] == 0)
        {
          negative = false;
        }
        else
          negative = true;
        //      if(negative){
        //        Serial.println("negative is true");
        //      }
        //      Serial.println();
        if (negative && (headdata[0] > 0))
        { // change number depending on how data array is set up, need to get negative, absolute, and
          turnspeed = 40;
          Serial.println("first if");
        }
        else if ((headdata[0] > 0))
        {
          turnspeed = 140;
          Serial.println("second if");
        }
        else
        {
          turnspeed = 90;
          Serial.println("else");
        }
        headServo.write(turnspeed);
        //        headServo.write(180);

        Serial.print("turnspeed: ");
        Serial.println(turnspeed);
      }

      // ================================ Motors ================================

      else
      {
        num[0] = data[1];
        num[1] = data[2];
        num[2] = data[3];
        num[3] = data[4];
        num[4] = data[5];
        float temp_left = atof(num);
        if (temp_left == 0)
        {
          zero_flagL = 1;
        }
        else if (abs(temp_left) <= 1)
        {
          left = temp_left; // only update value if it is valid (between -1 and 1)
          zero_flagL = 0;
        }

        num[0] = data[7];
        num[1] = data[8];
        num[2] = data[9];
        num[3] = data[10];
        num[4] = data[11];
        float temp_right = atof(num);
        if (temp_right == 0)
        {
          zero_flagR = 1;
        }
        else if (abs(temp_right) <= 1)
        {
          right = temp_right; // only update value if it is valid (between -1 and 1)
          zero_flagR = 0;
        }

        for (int i = 0; i < 13; i++)
          Serial.print(data[i]);
      }
      Serial.println();
    }
  }

  if (head)
  {
    // turnspeed = 90;
    headServo.write(turnspeed);
    delay(60);
    // head = false;
    // delay(60);
  }
  else
  {
    // set directions
    cw_R = (right > 0);
    cw_L = (left < 0);

    // ================================ PID ================================

    // scale setpoints to PID domain
    pid_setpoint_L = abs(left);
    pid_setpoint_R = abs(right);

    // take in RPM input from driver - analog to PID
    pid_input_L = analog_to_PID(analogRead(rpm_pin_L));
    pid_input_R = analog_to_PID(analogRead(rpm_pin_R));

    // write pwm to drivers with PID control enabled

    //  Serial.println("hello");

    left_pwm = PID_to_pwm(pid_output_L+pid_setpoint_L);
    right_pwm = PID_to_pwm(pid_output_R+pid_setpoint_R);
    //left_pwm = 100;
    //right_pwm = 100;

    if (zero_flagL && zero_flagR)
    {
      digitalWrite(cw_pin_R, 0);  // Direction --forward
      digitalWrite(cw_pin_L, 0);  // Direction
      digitalWrite(ccw_pin_R, 0); // Direction
      digitalWrite(ccw_pin_L, 0); // Direction
    }
    else
    {
      digitalWrite(cw_pin_R, cw_R);   // Direction --forward
      digitalWrite(cw_pin_L, cw_L);   // Direction
      digitalWrite(ccw_pin_R, !cw_R); // Direction
      digitalWrite(ccw_pin_L, !cw_L); // Direction
    }

    // hard cap on pwm output to drivers
    if (left_pwm > 253)
      left_pwm = 253;
    if (right_pwm > 253)
      right_pwm = 253;
    // send pwm to drivers
    analogWrite(pwm_pin_L, left_pwm);  // 253 max
    analogWrite(pwm_pin_R, right_pwm); // 253 max

    // must call compute pid every loop - will only actually run every SetSampleTime ms
    pid_R->Compute();
    pid_L->Compute();

    counter++;
  }
}

void send(char type[5], const uint8_t* msg, uint32_t msg_len, uint8_t* send_buffer) {
  uint32_t written = r2p_encode(type, msg, msg_len, send_buffer, 2048);
  Serial1.write(send_buffer, written);
  Serial.println("Sent");
  for (i=0; i<3; i++) {
    Serial.println(send_buffer[i],HEX);
  }
  //Serial.println("NUMBER OF BYTES WRITTEN: " + String(written));
}
