import serial
import time 
#import R2Protocol2 as r2p
import c1c0_liam_jonas_protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)
while(1):
	x = ser.read_until(expected = b'\xd2\xe2\xf2')
	s = r2p.decode(x,10)
	print(s)
	
