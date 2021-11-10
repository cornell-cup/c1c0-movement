
import serial
import time 
#import R2Protocol2 as r2p
import modified_protocol2 as r2p
ser = serial.Serial(
    port = '/dev/ttyTHS1',
    baudrate = 9600,
)
address = 4
address2 = 8
data = b"\x09\x09"
data2 = b"\x0a\x0a"
data3 = b"\x0b\x0b" 
data4 = b"\x0f\x0f" 
while(1):    
    print("controller1")
    msg = r2p.encode(b"Lite",address.to_bytes(1,'big'),data)
    print(msg)
    ser.write(msg)
    time.sleep(2)
    
    print("controller2")
    msg = r2p.encode(b"Dark",address2.to_bytes(1,'big'),data2)
    print(msg)
    ser.write(msg)
    time.sleep(2)

    print("controller1")
    msg = r2p.encode(b"Lite",address.to_bytes(1,'big'),data3)
    print(msg)
    ser.write(msg)
    time.sleep(2)

    print("controller2")
    msg = r2p.encode(b"Dark",address2.to_bytes(1,'big'),data4)
    print(msg)
    ser.write(msg)
    time.sleep(2)


