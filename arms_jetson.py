import serial
import time

ser = serial.Serial(
	port = '/dev/ttyTHS1',
	baudrate = 9600,
)

#ser.reset_input_buffer()

def printMoves(x):
	print("J" + str(x+1) + ": " + str(serial1[x+2]))

print(ser.name)

startseq = (65534).to_bytes(2, 'big')
endseq = (65533).to_bytes(2, 'big')

while(1):
	serial1 = ser.read(10)
	print(serial1)
	s = None
	while s is None:
		if serial1[:2] == startseq and serial1[-2:] == endseq:
			s = serial1[2:-2]
		else:
			serial1 = serial1[9].to_bytes(1,'big') + serial1[:9]
	
	for i in range(6):
		printMoves(i)
	
	
	# motor1str = str(hex(s[0])+hex(s[1]))
	# motor2str = str(hex(s[2])+hex(s[3]))
	# motor3str = str(hex(s[4])+hex(s[5]))
	# motor4str = str(hex(s[6])+hex(s[7]))
	# motor5str = str(hex(s[8])+hex(s[9]))
	# motor6str = str(hex(s[10])+hex(s[11]))
	
	# motor1pos = int(motor1str.replace('0x', ''),16)
	# motor2pos = int(motor2str.replace('0x', ''),16)
	# motor3pos = int(motor3str.replace('0x', ''),16)
	# motor4pos = int(motor4str.replace('0x', ''),16)
	# motor5pos = int(motor5str.replace('0x', ''),16)
	# motor6pos = int(motor6str.replace('0x', ''),16)
	
	# print(motor1pos)
	# print(motor2pos)
	# print(motor3pos)
	# print(motor4pos)
	# print(motor5pos)
	# print(motor6pos)
	
	#ser.write(bytes(3024))
	
	#print(i) 4176 14408
	# compare i to the target values :P


