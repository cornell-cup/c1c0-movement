
import random
import serial
import time 
import modified_protocol2 as r2p
import select
import sys
import threading
import Jetson.GPIO as GPIO
ser = serial.Serial(
        port = '/dev/ttyTHS1',
        baudrate = 9600,
)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)
address = 4
address2 = 8
data = b"\x09\x09"
data2 = b"\x08\x08"
i = 0

def send():
	while(1):
		msg = r2p.encode(b"rqst",(4).to_bytes(1,'big'),data)
		ser.write(msg)
		print("sending " + str(msg))
		time.sleep(1)
		msg = r2p.encode(b"rqst",(4).to_bytes(1,'big'),data2)
		ser.write(msg)
		time.sleep(1)
		
def receive():
	while(1):
		x = ser.read_until(expected = b'\xd2\xe2\xf2' )
		s = r2p.decode(x)
		print(str(s[0]))
		time.sleep(1)
		if(s[3] == 4):
			GPIO.output(7,GPIO.HIGH)
			time.sleep(.5)
			GPIO.output(7,GPIO.LOW)

print("Start of cycle")
x =threading.Thread(target=send)
y =threading.Thread(target=receive)
x.start()
y.start()

	
	


