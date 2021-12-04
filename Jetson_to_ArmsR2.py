import serial
import sys
import time

sys.path.insert(0, './c1c0-movement/Locomotion/')
import R2Protocol2

ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)

def writeToSerial(writeArray):
	'''
	This functions writes a byte array from the Jetson to the Precise
	Arm using R2Protocol. 
	
	The input 'writeArray' is length 6 for each joint on the Precise Arm.
	Each index of the array is the target angle (in degrees) for each 
	joint.
	
	For example, to get Joints 2 and 3 to move to angles 80 and 90. 
	Say the previous writetoSerial command was [10,20,30,40,50,60]. 
	The next command would then be: writeToSerial([10,80,90,40,50,60]).
	So, change the indices you want to change, and keep the previous
	angles for the joints you don't want to move.
	'''
	# PRM = Precise Arm
	# cast writeArray from int to byte, encode the array using the R2Protocol
	write_byte = R2Protocol2.encode(bytes('PRM','utf-8'), bytearray(writeArray))
	print(write_byte)
	# send the encoded array across the Jetson Serial lines
	ser.write(write_byte)
	
	

# For Debugging Purposes 	
# arm should move from start position to 60 to 100 to 20 then stop
array1 = [10,60,30,40,50,60]
array2 = [10,100,30,40,50,60]
array3 = [10,20,30,40,50,60]


writeToSerial(array1)
ser.reset_input_buffer()
print("first send: ", array1[1])
time.sleep(5)
writeToSerial(array2)
ser.reset_input_buffer()
print("second send:", array2[1])
time.sleep(5)
writeToSerial(array3)
ser.reset_input_buffer()
print("third send: ", array3[1])

