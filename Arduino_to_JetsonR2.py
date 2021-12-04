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
	If the value is 1452, then the encoder is not powered or there is 
	a wiring issue. 
	'''
	# initialize the array 
	encoderAngle = [0,0,0,0,0,0]
	try:
		while True:
			 # length of message plus 16
			good_data = False
			while (not good_data):
				ser_msg = ser.read(28)
				msgtype, msg, status = r2p.decode(ser_msg)
				# print(msg.hex())
				print(status)
				print(ser_msg)
				if (status):
					good_data = True
				else:
					ser.reset_input_buffer()
			for i in range(0, 12, 2):
				encoderAngle[i//2] = (msg[i]<<8) | msg[i+1]
			print(encoderAngle)
		
	except KeyboardInterrupt:
            ser.close()
	
	
if __name__ =='__main__':
	init_serial('/dev/ttyTHS1', 38400)
	read_encoder_values()
	
	
	
			
