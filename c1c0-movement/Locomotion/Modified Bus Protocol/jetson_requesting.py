
import random
import serial
import time 
import modified_protocol2 as r2p
import select
import sys
ser = serial.Serial(
        port = '/dev/ttyTHS1',
        baudrate = 9600,
)
address = 4
address2 = 8
data = b"\x09\x09"
i = 0
while(1):
	print("Start of cycle")
	print(i)
	msg = r2p.encode(b"rqst",(4).to_bytes(1,'big'),data)
	ser.write(msg)
	time.sleep(.5)
	if(i % 5 == 0):
		print("sending ...")
	while ser.in_waiting:
		print("Reading Message")
		x = ser.read(20)
		##x = ser.read_until(expected = b'\xd2\xe2\xf2' )
		s = r2p.decode(x)
		print("Message received: ", s)
	i = i + 1
	print()
	time.sleep(.5)
	
	


