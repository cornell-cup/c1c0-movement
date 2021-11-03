
import serial
import time 
import modified_protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1', ##setting up jetson serial port at 9600 baud rate
	baudrate = 9600,
)
ID = 4
while(1): ## loop takes in data sent from jetson. In the decode method, the message is parsed if the ID variable matches the address of the sent message
    x = ser.read_until(expected = b'\xd2\xe2\xf2')
    s = r2p.decode(x,ID)
    print(s)

