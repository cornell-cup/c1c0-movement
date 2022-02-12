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
        baudrate = 38400,
)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)
address = 4
address2 = 8
data = b"\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09\x09"
data2 = b"\x08\x08"
type1 = b"rqst"
i = 0
def send(type,address,data):
        msg = r2p.encode(type,(address).to_bytes(1,'big'),data)
        ser.write(msg)
        print("sending " + str(type1) + "," + str(data) + "," + str(address))
tim = 0
for i in range(100):                                           	
        #print("cycle: ",i)
        start = time.time()
        send(type1,3,data)
        ##waiting for message after sending
        while(1):
                x = ser.read_until(expected = b'\xd2\xe2\xf2' )
                s = r2p.decode(x)
                print("I'm receiving:",s)
                break
        send(type1,4,data)
        ##waiting for message after sending
        while(1):
                x = ser.read_until(expected = b'\xd2\xe2\xf2' )
                print(x)
                s = r2p.decode(x)
                print("I'm receiving:",s)
                break
        end = time.time()
        #print()
        tim += end-start 
print(tim/200)
