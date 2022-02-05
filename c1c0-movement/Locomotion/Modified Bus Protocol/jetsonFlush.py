import random
import sys
import serial
import time 
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
	)
while(1):
	ser.read()
