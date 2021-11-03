import serial
import sys
import time

'''
Arduino to Jetson Communication with R2 Protocol
'''

sys.path.insert(0, './c1c0-movement/Locomotion/')
import R2Protocol2 as r2p

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
	
	
if __name__ =='__main__':
	init_serial('/dev/ttyTHS1', 9600)
	
	try:
            while True:
                start = time.time()
                ser_msg = ser.read(22) # length of message plus 16
                for i in range(0, len(ser_msg), 32):
                    r2p.decode(ser_msg[i:i+32])
                    print(ser_msg[i:i+32])
				
	except KeyboardInterrupt:
            ser.close()
			
