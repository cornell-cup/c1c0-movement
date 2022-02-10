
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
type1 = b"rqst"
i = 0
lock = threading.Lock()
def send():
	while(1):
		for i in range(1,4):
		#i=2
			lock.acquire()
			msg = r2p.encode(type1,(i).to_bytes(1,'big'),data)
			ser.write(msg)
			print("sending " + str(type1) + "," + str(data) + "," + str(i))
			time.sleep(1)
			msg = r2p.encode(b"rqst",(i).to_bytes(1,'big'),data2)
			ser.write(msg)
			print("sending " + str(type1) + "," + str(data2) + "," + str(i))
			lock.release()
			time.sleep(1)
def receive():
	while(1):
		
		
		x = ser.read_until(expected = b'\xd2\xe2\xf2' )
		lock.acquire()
		print("I'm receiving")
		s = r2p.decode(x)
		print(str(s[3])) 
		print(str(s[0]))
		time.sleep(.5)
		if(s[3] == 4):
			GPIO.output(7,GPIO.HIGH)
			time.sleep(.5)
			GPIO.output(7,GPIO.LOW)
		lock.release()
		time.sleep(1)
print("Start of cycle")
x =threading.Thread(target=send)
y =threading.Thread(target=receive)
x.start()
y.start()

	
	


