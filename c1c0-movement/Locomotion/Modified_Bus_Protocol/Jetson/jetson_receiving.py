
import serial
import time 
import modified_protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1', ##setting up jetson serial port at 9600 baud rate
	baudrate = 115200,
)
ID = 4
ser.reset_input_buffer()
while(1): ## loop takes in data sent from jetson. In the decode method, the message is parsed if the ID variable matches the address of the sent message
    ##x = ser.read(13)
    x = ser.read_until(expected = b'\xd2\xe2\xf2')
    print('undecoded:',x)
    s = r2p.decode(x)
    print('decoded:',s)

