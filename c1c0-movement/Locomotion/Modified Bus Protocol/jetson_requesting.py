
import random
import serial
import time 
import modified_protocol2 as r2p
ser = serial.Serial(
        port = '/dev/ttyTHS1',
        baudrate = 9600,
)
address = 4
address2 = 8
data = b"\x09\x09"
while(1):
		req = input();
		if(int(req) == 1):
				print("sending ...")
				msg = r2p.encode(b"rqst",(4).to_bytes(1,'big'),data)
				ser.write(msg)
		x = ser.read_until(expected = b'\xd2\xe2\xf2')
		s = r2p.decode(x)
 
