
import serial
import time 
#import R2Protocol2 as r2p
import c1c0_liam_jonas_protocol2 as r2p
ser = serial.Serial(
        port = '/dev/ttyAMA0',
	baudrate = 9600,
)
address = 4;
type1 = "ON"
data = "ON"
while(1):
	for i in range(6):
		
		print("ON")
		msg = r2p.encode(b"ON",address.to_bytes(1,'big'),b"\x00a\x00a")
		print(msg)
		ser.write(msg)
		time.sleep(3)
		##print("OFF")
		##msg = r2p.encode(b"DOE",address.to_bytes(1,'big'),b"\x00b\x00a")
		##print(msg)
		##msg = r2p.encode(b"DOE",b"\x00b\x00a")
		##print(msg)
		##ser.write(msg)
		##time.sleep(3)
