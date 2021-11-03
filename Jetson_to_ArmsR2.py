import serial
import sys

sys.path.insert(0, './c1c0-movement/Locomotion/')
import R2Protocol2

ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)

# write timeout set to 10 seconds
write_timeout = 10
send_it = [0,1,2,3,4,5] # the array to send to the Arduino
# PARM = Precise Arm
write_byte = R2Protocol2.encode(bytes('PARM','utf-8'), bytearray(send_it))
ser.write(write_byte)
