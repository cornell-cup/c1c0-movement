import random
import serial
import time 
#import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyTHS1',
    baudrate = 38400,
)
address = 3
address2 = 8
data = b"\x09"


while(1):
        data += b"\x09"
        print("controller1")
        msg = r2p.encode(b"Lite",(3).to_bytes(1,'big'),data)
        #msg2 = r2p.encode(b"Lite",(i).to_bytes(1,'big'),data3)
        print(msg)
        ser.write(msg)
        #ser.write(msg2)
        time.sleep(.2)


