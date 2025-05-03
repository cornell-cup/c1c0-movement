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
#include "joint.h"

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
volatile bool update_angspd = true;
bool interrupt_status = true;
bool manual = true;
bool dir_bool;
int ang_spd;
byte dist_data = 0;
int turnspeed;
bool head = false;
bool carriage = false;
int headdata[3];

int counter = 0;

bool zero_flagL;
bool zero_flagR;

// pin defintions
int pwm_pin_R = 11; //11
int pwm_pin_L = 3;
int cw_pin_R = 12; //12
int ccw_pin_R = 13;//13
int rpm_pin_R = A0;
int cw_pin_L = 4;
int ccw_pin_L = 5;
int rpm_pin_L = A1;
int max_pwm = 100;// 255 caused the motor drivers to stall, so we set max as 253

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

// Create a new servo object:
Servo myservo;

// Define the servo pin:
#define servoPin 7

encoder_t head_encoder = {.cs = 44, .resolution = 14, .correctDir = 0, 
                          .target_angle = 0, .max_angle = 179, .min_angle = -179};

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("starting setup");
  Serial3.begin(115200);

  myservo.attach(servoPin);
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
  headServo.attach(6, 556, 2410); // attaches the servo on pin 10 to the servo object, PWM range between 556-2410 for the HS-755HB (change for different servos)
                                   //    headServo.attach(10);


  
  while (Serial1.available() > 0) {
    Serial1.read();
  }

  // acknowledgement message
  for(int i = 0; i < 3; i++) {
    msg_data_buffer[i] = (uint8_t) msg[i];
  }
  delay(500);
  init_encoder(&head_encoder);

  Serial.println("setup");

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 65536 - (62500/8);          // preload timer 65536-(16MHz/256/2Hz)
  TCCR1B |= (1 << CS12);  // 256 prescaler
  //TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts();           // enable all interrupts

}

ISR(TIMER1_OVF_vect) {        // interrupt service routine 
TCNT1 = 65536 - (62500/8);   // preload timer

switch (dist_data) {
  case 1: // sensor 1 is reading closest object
    head_encoder.target_angle = 0;
    update_angspd = true;
    break;
  case 2: // sensor 2 is reading closest object
    head_encoder.target_angle = 30;
    update_angspd = true;
    break;
  case 3: // sensor 3 is reading closest object
    head_encoder.target_angle = 60;
    update_angspd = true;
    break;
  case 4: // sensor 4 is reading closest object
    head_encoder.target_angle = 90;
    update_angspd = true;
    break;
  case 5: // sensor 5 is reading closest object
    head_encoder.target_angle = 120;
    update_angspd = true;
    break;
  case 6: // sensor 6 is reading closest object
    head_encoder.target_angle = 150;
    update_angspd = true;
    break;
  case 7: // sensor 7 is reading closest object
    head_encoder.target_angle = 179;
    update_angspd = true;
    break;
  case 8: // sensor 8 is reading closest object
    head_encoder.target_angle = -150;
    update_angspd = true;
    break;
  default:
    //Serial.println("stopping servo movement");
    headServo.write(90);
  }

}

uint8_t num[5];

void loop()
{


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
        carriage = false;
        manual = true;
        if (interrupt_status){
          noInterrupts();
          TIMSK1 &= ~(1 << OCIE1A);
          interrupts(); 
          interrupt_status = false; 
        }     

        //   Serial.println("manual head");
        // if (int(data[10]) - 48 == 9){
        //   manual = false;
        //   head = false;
        //   carriage = true;
        //   Serial.println("auto head");
        // }else{
        //   head = true;
        //   carriage = false;
        //   manual = true;
        //   Serial.println("manual head");
        // }
      } else if (data[0] == 'c' && data[1] == 'a' && data[2] == 'r' && data[3] == 'r'){
        head = false;
        carriage = true;
        Serial.print("carriage activated");

      } else if (data[0] == 'a' && data[1] == 'u' && data[2] == 't' && data[3] == 'o'){
        manual = false;
        head = true;
        carriage = true;
    
        noInterrupts();
        TIMSK1 |= (1 << TOIE1);
        interrupts();

        interrupt_status = true;      
        Serial.println("auto head");
        Serial.println("hello there");

      }  else {
        head = false;
        carriage = false;
        manual = false;
        noInterrupts();
      }
      

      Serial.print("Head?: ");
      Serial.println(head);
      Serial.print("Headdata: ");
      Serial.print(data[0]);
      Serial.print(data[1]);
      Serial.print(data[2]);
      Serial.print(data[3]);
      Serial.println("");
      // ================================ Head Servo ================================

      if (head && manual)
      {
        getPositionSPI(&head_encoder);
        Serial.print("cur head angle: ");
        Serial.println(head_encoder.current_angle);
        headdata[0] = int(data[10]) - 48;
        headdata[1] = int(data[11]) - 48;
        headdata[2] = int(data[12]) - 48;
        for (int i = 0; i < 3; i++)
        {
          Serial.println(headdata[i]);
        }
        absolute = headdata[1];
        //Serial.println(headdata[2]);
        if (headdata[2] == 0)
        {
          negative = false;
        }
        else
          negative = true;

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
          turnspeed = 92;
          Serial.println("else");
        }
        headServo.write(turnspeed);
        //        headServo.write(180);

        Serial.print("turnspeed: ");
        Serial.println(turnspeed);
      }

      // ++++++++++++++++++++++++++++++++ carriage ++++++++++++++++++++++++++++++

      else if (carriage){
        Serial.println(myservo.read());
        if(myservo.read() >= 85){
          myservo.write(60);
        }
        else{
          myservo.write(88);
        }
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

  if (head && manual)
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
    if (left_pwm > max_pwm)
      left_pwm = max_pwm;
    if (right_pwm > max_pwm)
      right_pwm = max_pwm;
    // send pwm to drivers
    analogWrite(pwm_pin_L, left_pwm);  // 253 max
    analogWrite(pwm_pin_R, right_pwm); // 253 max

    // must call compute pid every loop - will only actually run every SetSampleTime ms
    pid_R->Compute();
    pid_L->Compute();

    counter++;
  }

// ==================== auto mode ==============
  setTargetAngle(&head_encoder, head_encoder.target_angle);

  if (!manual && head)
  { 
    if (Serial3.available()){
      dist_data = Serial3.read();
      // Serial.print("sensor data: ");
      // Serial.println(dist_data);
    }
    
    //Serial.println("hellos");
    getPositionSPI(&head_encoder);
    // Serial.print("tar head angle: ");
    // Serial.println(head_encoder.target_angle);
    //Serial.println("hi");
    float ang_mag = abs(head_encoder.target_angle - head_encoder.current_angle);
    
      // Serial.println("direction: ");
      // Serial.println(dir_bool);

    if ((head_encoder.current_angle <= head_encoder.target_angle + 3) && (head_encoder.current_angle >= head_encoder.target_angle - 3)){
      Serial.println(head_encoder.current_angle);
      head_encoder.correctPos = true;
      headServo.write(90);
      update_angspd = true;
    } else {
      head_encoder.correctPos = false;
    }

    //Serial.println(update_angspd);
    if (update_angspd && !head_encoder.correctPos) {
      ang_spd = ang_to_turnspd(ang_mag);
      dir_bool = head_rotation_dir(head_encoder.current_angle, head_encoder.target_angle);
      // Serial.println("direction: ");
      // Serial.println(dir_bool);

      update_angspd = false;
      // Serial.print("setting update_ang_sped to: ");
      // Serial.println(update_angspd);
      float bias = (abs(head_encoder.current_angle - head_encoder.target_angle)/180.0) * 60;
      if (head_encoder.current_angle < head_encoder.target_angle){
        if (dir_bool){
          // Serial.print("servo write: ");
          // Serial.println(20);
          // Serial.print("current head angle: ");
          // Serial.println(head_encoder.current_angle);
          
          headServo.write(60 - bias);
         }else{
          // Serial.print("servo write: ");
          // Serial.println(150);
          // Serial.print("current head angle: ");
          // Serial.println(head_encoder.current_angle);
          //bias = (abs(head_encoder.current_angle - head_encoder.target_angle)/180.0) * 60
          headServo.write(120 + bias);
        }
      } else if (head_encoder.current_angle > head_encoder.target_angle){
        if (dir_bool){
          // Serial.print("servo write: ");
          // Serial.println(150);
          // Serial.print("current head angle: ");
          // Serial.println(head_encoder.current_angle);
          headServo.write(120 + bias);
        }else{
          // Serial.print("servo write: ");
          // Serial.println(20);
          // Serial.print("current head angle: ");
          // Serial.println(head_encoder.current_angle);
          headServo.write(60 - bias);
        }
      }
        
    }
  } 

}

bool head_rotation_dir(float current, float target){
  if (abs(current - target) <= 180){
    return true;
  }
  return false;
}

inline int ang_to_turnspd(float ang_mag){
  return (int) ((ang_mag/8.0) - 1); // divided by wo bc (ang_diff/360.0) * 45.0
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
