import serial
import time 
import R2Protocol2 as r2p
#import c1c0_liam_jonas_protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)
while(1):
	print(1)
	x = ser.read()
	
	
