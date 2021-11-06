
import serial
import time 
#import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyTHS1',
    baudrate = 9600,
)
address = 4;
type1 = b"ON"
data = b"\x10a\x00a"
while(1):    
    print("ON")
    msg = r2p.encode(b"ON",address.to_bytes(1,'big'),data)
    print(msg)
    ser.write(msg)
    time.sleep(3)
        
