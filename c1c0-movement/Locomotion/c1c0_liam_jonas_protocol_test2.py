import serial
import time 
import R2Protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)
type1 = "ON"
data = "ON"
while(1):
	for i in range(6):
		
		print("ON")
		msg = r2p.encode(b"ON",b"\x00a\x00a")
		ser.write(msg)
		time.sleep(3)
		print("OFF")
		msg = r2p.encode(b"OFF",b"\x00a\x00a")
		ser.write(msg)
		time.sleep(3)
		
