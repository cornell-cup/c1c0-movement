
import serial
import time 
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyS0', ##port on jetson
    baudrate = 9600,
)
address = 8
type1 = "ON"
data = "\x00a\x00a"
while(1):      
    ## loop encodes a message with address 8 and sends it through the serial port to the microcontrollers in the bus. 
    ##the message will be parsed by every microcontroller matching this address
    print("ON") 
    msg = r2p.encode(b"ON",address.to_bytes(1,'big'),b"\x00a\x00a")
    print(msg)
    ser.write(msg)
    time.sleep(2)

