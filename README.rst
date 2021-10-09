*****
c1c0-movement
*****
Jetson and microcontroller (Arduino) code for locomotion and arms movement.

Jetson:
#######
* Server processes taking input for locomotion and arms movement
* Communicates encoded information to locomotion and arms microcontrollers using R2Protocol
  via pySerial.
* Will need to register responses from either microcontroller to keep track of either system's status

Microcontrollers (Arduinos):
######

* Locomotion microcontroller accepts R2Protocol-encoded messages on a serial line
    * Provides reference to each motor
    * Will need to handle feedback control to coordinate between the two
    * Will need to ping Jetson back when accepting and executing commands
  
* Arms microcontroller to accept R2Protocol-encoded messages on a serial line
    * Should receive movement amounts from the Jetson
    * Using rotary encoder feedback, move the correct angle amount
    * Message jetson on start and finish


Installation:
######
For source distributions:
::
    python setup.py sdist
    pip install dist/[newly_genereated_file]


For build distributions:
::
    python setup.py bdist
    pip install dist/[newly_genereated_file]