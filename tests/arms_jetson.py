import serial
import time

ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)

def printMoves(x):
	print("J" + str(x+1) + ": " + str(s[x]))

print(ser.name)

startseq = (65534).to_bytes(2, 'big')
endseq = (65533).to_bytes(2, 'big')

while(1):
	print(ser.in_waiting)
	serial1 = ser.read(19)
	ser.reset_input_buffer()
	print(serial1)
	
	try:
		start = serial1.index(startseq)
		serial1[start+8] == endseq
		s = serial1[start+2:start+8]
	except:
		time.sleep(3)
		serial1 = ser.read(19)
		ser.reset_input_buffer()
		start = serial1.index(startseq)
		serial1[start+8] == endseq
		s = serial1[start+2:start+8]
	
	print(s)

	# ~ s = None
	# ~ while s is None:
		# ~ if serial1[:2] == startseq and serial1[-2:] == endseq:
			# ~ s = serial1[2:-2]
		# ~ else:
			# ~ serial1 = serial1[9].to_bytes(1,'big') + serial1[:9]
	
	#serial1.clear()
	
	for i in range(6):
		printMoves(i)
	
# find where fffe is
# add 8 to that index
# if that index and the one after is fffd, set s to the in between



