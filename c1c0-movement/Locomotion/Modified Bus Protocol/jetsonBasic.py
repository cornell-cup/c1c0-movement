import random
import serial
import time

ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)

while(1):
	msg =b'\0x12'
	ser.write(msg)
	time.sleep(1)
