/*
  - Read x and y values from Jetson
  - Calculate desired angle/motor speeds
  - Move motors
  - Read encoder values
  - Change speeds based on feedback
  
 */

void setup() {
  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void loop() {
  analogWrite(3, 25); //253 max
  digitalWrite(8, HIGH);
  int speed = analogRead(A0); //rpm scaled by 15
  int avspeed = analogRead(A1);
  Serial.println(speed);
  Serial.println(avspeed);
  Serial.println("");
}
