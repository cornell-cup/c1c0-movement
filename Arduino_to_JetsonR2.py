import serial
import sys
import codecs

'''
Arduino to Jetson Communication with R2 Protocol
'''

sys.path.insert(0, './c1c0-movement/Locomotion/')
import R2Protocol2 as r2p
file1 = open("file1.txt", "w")

ser = None
 
def init_serial(port,baud):
	'''
	Initializes the serial port, usually set baud to 9600
	'''
	global ser, startseq, endseq
	ser =serial.Serial(port,baud)
	
def close_serial():
	'''
	Closes the serial port.
	'''
	global ser
	ser.close()
	
def read_encoder_values():
	'''
	Returns 6 encoder values i(n decimal) as an array.
	'''
	
	
if __name__ =='__main__':
	init_serial('/dev/ttyTHS1', 9600)
	
	# initialize the array 
	encoderAngle = [0,0,0,0,0,0]
	try:
		while True:
			ser_msg = ser.read(22) # length of message plus 16
			for i in range(0, len(ser_msg), 32):
				msgtype, msg, status = r2p.decode(ser_msg[i:i+32])
			    # print(msg.hex())
				
				for i in range(6):
					encoderAngle[i] = int(msg[i:i+1].hex(), 16)
					
				print(encoderAngle)
			
	except KeyboardInterrupt:
            ser.close()
			
