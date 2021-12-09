import random
import sys
import serial
import time 
sys.path.append('Locomotion/R2Protocol2.py')
import R2Protocol2 as r2p
##import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyTHS1',
    baudrate = 115200,
)
address = 4
address2 = 8
data = b"\x09\x09"
data2 = b"\x0a\x0a"
data3 = b"\x0b\x0b" 
data4 = b"\x0f\x0f" 
while(1):
	'''
    for i in range(0,6):
        print("controller1")
        msg = r2p.encode(b"Lite",(3).to_bytes(1,'big'),data3)
        #msg2 = r2p.encode(b"Lite",(i).to_bytes(1,'big'),data3)
        print(msg)
        ser.write(msg)
        #ser.write(msg2)
        time.sleep(.1)
    for i in range(0,6):
        print("controller1")
        msg = r2p.encode(b"Lite", (3).to_bytes(1,'big'),data)
        #msg2 = r2p.encode(b"Lite", (i).to_bytes(1,'big'),data)
        print(msg)
        ser.write(msg)
        #ser.write(msg2)
        time.sleep(.1)'''
	msg = r2p.encode(b"Lite",data3)
	msg2 = r2p.encode(b"Lite",data)
	print(msg)
	ser.write(msg)
	time.sleep(.5)
	print(msg2)
	ser.write(msg2)
	time.sleep(.5)
       



