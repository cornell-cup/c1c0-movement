# c1c0-movement
Contains locomotion code to control C1C0's foot motors.

## locomotion_control.ino:
- Locomotion microcontroller accepts R2Protocol-encoded messages on a serial line
  - Provides reference to each motor
  - Will need to handle feedback control to coordinate between the two (TODO)
  - Will need to ping Jetson back when accepting and executing commands (TODO)
