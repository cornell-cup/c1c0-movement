
import serial
import time 
#import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
)
while(1):
    x = ser.read_until(expected=b'\xd2\xe2\xf2')
    s = r2p.decode(x,8)
    print(s)

