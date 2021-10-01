import serial
import time 
import R2Protocol2 as r2p
ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)
while(1):
	for i in range(6):
		data = i
		print(data)
		msg = r2p.encode(12,data)
		ser.write(msg)
		print(bytes[data])
		time.sleep(1)
