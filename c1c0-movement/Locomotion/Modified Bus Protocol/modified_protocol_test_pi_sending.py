
import serial
import time 
#import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
)
address = 8;
type1 = "ON"
data = "ON"
while(1):       
        print("ON")
        msg = r2p.encode(b"ON",address.to_bytes(1,'big'),b"\x00a\x00a")
        print(msg)
        ser.write(msg)
        time.sleep(2)
        ##print("OFF")
        ##msg = r2p.encode(b"DOE",address.to_bytes(1,'big'),b"\x00b\x00a")
        ##print(msg)
        ##msg = r2p.encode(b"DOE",b"\x00b\x00a")
        ##print(msg)
        ##ser.write(msg)
        ##time.sleep(3)
