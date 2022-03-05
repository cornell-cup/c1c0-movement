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
address = 3
address2 = 4
data = b'\x09'
for i in range(9):
    data += b'\x09'

type1 = b"rqst"
recvData;

i = 0
def send(type,address,data):
        msg = r2p.encode(type,(address).to_bytes(1,'big'),data)
        ser.write(msg)
        #print("sending " + str(type1) + "," + str(data) + "," + str(address))
        print("sending")
tim = 0
for i in range(100):                                           	
        start = time.time()
        while(1):
                send(type1,address,data)
                ##waiting for message after sending
                while(1):
                        x = ser.read_until(expected = b'\xd2\xe2\xf2' )
                        s = r2p.decode(x)
                        recvData = s[1]                        
                        s3.decode('ascii')
                        print("I'm receiving:",s)
                        break
                if(recvData == b'\0x1\0x2\0x3'):
                        send(type1,address2,data)
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
