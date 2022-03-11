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
data = b"\x09\x09\x09"


while(1):
        msg = r2p.encode(b"Lite",(3).to_bytes(1,'big'),data)
        print("sending")
        ser.write(msg)
        #while(1):
        #        x = ser.read_until(expected = b'\xd2\xe2\xf2' )
        #        s = r2p.decode(x)
        #        print("I'm receiving:",s)
        #        break
        time.sleep(1)


