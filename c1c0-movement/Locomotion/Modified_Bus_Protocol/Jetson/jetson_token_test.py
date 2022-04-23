import random
import serial
import time 
import modified_protocol2 as r2p
import select
import sys
import threading

sys.path.append("/home/cornellcup/c1c0-ece")
import TEST_API as sns

ser = sns.init_serial('/dev/ttyTHS1', 38400)
ser.set_buffer_size(rx_size = 2048, tx_size = 2048)

def send_token(permission,addr):
    msg = r2p.encode(b"SNS", (addr).to_bytes(1,'big'),(permission.to_bytes(1,'big')))
    ser.write(msg)
    ser.flush()
	
send_token(0,3)
send_token(0,2)

while True:
	send_token(1,3)
	for i in range(10):
		x = ser.read_until(expected = b'\xd2\xe2\xf2' )
		s = r2p.decode(x) 
		print(s)
	send_token(0,3)
	time.sleep(.05)
	ser.reset_input_buffer()
	time.sleep(.05)
    
	send_token(1,2)
	for i in range(10):
		x = ser.read_until(expected = b'\xd2\xe2\xf2' )
		s = r2p.decode(x) 
		print(s)
	send_token(0,2)

	time.sleep(.05)
	ser.reset_input_buffer()
	time.sleep(.05)
    
    # ~ sns.send_token(1, 200)
    # ~ for i in range(10):
        # ~ sns.decode_arrays()
        # ~ tb1 = sns.get_array("TB1")
        # ~ tb2 = sns.get_array("TB2")
        # ~ ldr = sns.get_array("LDR")
        # ~ imu = sns.get_array("IMU")
    # ~ sns.send_token(0,200)
    # ~ ser.flush()
    # ~ time.sleep(.05)
    # ~ ser.reset_input_buffer()
    # ~ time.sleep(.05)
    
    # ~ send_token(1,3)
    # ~ for i in range(10):
        # ~ x = ser.read_until(expected = b'\xd2\xe2\xf2' )
        # ~ s = r2p.decode(x) 
        # ~ print(s)

    # ~ send_token(0,3)
    # ~ time.sleep(.05)
    # ~ ser.reset_input_buffer()
    # ~ time.sleep(.05)
    
	

