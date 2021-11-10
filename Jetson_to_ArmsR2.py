import serial
import sys

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
	# PARM = Precise Arm
	# cast writeArray from int to byte, encode the array using the R2Protocol
	write_byte = R2Protocol2.encode(bytes('PARM','utf-8'), bytearray(writeArray))
	# send the encoded array across the Jetson Serial lines
	ser.write(write_byte)
	
# For Debugging Purposes 	
writeToSerial([10,20,30,40,50,60])
