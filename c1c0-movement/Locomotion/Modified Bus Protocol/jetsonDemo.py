import random
import sys
import serial
import time 
sys.path.append('Locomotion/R2Protocol2.py')
##import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)
address = 4
address2 = 8
data = b"\x09\x09"
data2 = b"\x0a\x0a"
data3 = b"\x0b\x0b" 
data4 = b"\x0f\x0f" 
while(1):
	print("What pattern would you like?")
	print("Options: linear, InOut, OutIn, or all")
	pattern = input("Pattern:" )	
	if(pattern == "linear"):
		for f in range(0,3):
			for i in range(2,6):
				msg = r2p.encode(b"Lite", (i).to_bytes(1,'big'),data)
				ser.write(msg)
				time.sleep(.2)
			for i in range(2,6):
				msg = r2p.encode(b"Lite", (i).to_bytes(1,'big'),data)
				ser.write(msg)
				time.sleep(.2)
	elif(pattern == "InOut"):
		for f in range(0,5):
			for i in range(0,2):
				msg = r2p.encode(b"Lite",(3-i).to_bytes(1,'big'),data3)
				msg2 = r2p.encode(b"Lite", (4+i).to_bytes(1,'big'),data3)
				ser.write(msg)
				ser.write(msg2)
				time.sleep(.1)
			for i in range(0,3):
				msg = r2p.encode(b"Lite", (3-i).to_bytes(1,'big'),data)
				msg2 = r2p.encode(b"Lite", (4+i).to_bytes(1,'big'),data)
				ser.write(msg)
				ser.write(msg2)
				time.sleep(.1)
	elif(pattern == "OutIn"):
		for f in range(0,5):
			for i in range(0,2):
				msg = r2p.encode(b"Lite",(5-i).to_bytes(1,'big'),data3)
				msg2 = r2p.encode(b"Lite", (2+i).to_bytes(1,'big'),data3)
				ser.write(msg)
				ser.write(msg2)
				time.sleep(.2)
			for i in range(0,3):
				msg = r2p.encode(b"Lite", (5-i).to_bytes(1,'big'),data)
				msg2 = r2p.encode(b"Lite", (2+i).to_bytes(1,'big'),data)
				ser.write(msg)
				ser.write(msg2)
				time.sleep(.2)
	elif(pattern == "all"):
		while(1):
			for f in range(0,5):
				msg = r2p.encode(b"Lite",(2).to_bytes(1,'big'),data3)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(3).to_bytes(1,'big'),data3)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(4).to_bytes(1,'big'),data3)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(5).to_bytes(1,'big'),data3)
				ser.write(msg)
				time.sleep(.5)
				msg = r2p.encode(b"Lite",(2).to_bytes(1,'big'),data)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(3).to_bytes(1,'big'),data)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(4).to_bytes(1,'big'),data)
				ser.write(msg)
				msg = r2p.encode(b"Lite",(5).to_bytes(1,'big'),data)
				ser.write(msg)
				time.sleep(.5)
	elif(pattern == "test"):
		for i in range(0,6):
			print(i)
			msg = r2p.encode(b"Lite",(i).to_bytes(1,'big'),data3)
			ser.write(msg)
			time.sleep(2)
		for i in range(0,6):
			print(i)
			msg = r2p.encode(b"Lite",(i).to_bytes(1,'big'),data)
			ser.write(msg)
			time.sleep(2)
	elif(pattern == "exit"):
		break
	else:
		print("That is not a command")
		print("Try again")
	for i in range(2,6):
		msg = r2p.encode(b"Lite",(i).to_bytes(1,'big'),data)
		ser.write(msg)
		time.sleep(.01)
		
			
			



