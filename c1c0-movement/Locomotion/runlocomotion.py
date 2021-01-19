# new version of runlocomotion.py
# runs two threads - one which sends commands 
# to HeadLocomotion, and one which reads from
# the serial line to assure data is being received

import threading
import locomotion

lock = threading.Lock()

def cmd_thread():

    while True:

    	lock.acquire()
        locomotion.run(1)
        lock.release()

def read_thread():
	
	while True:

		lock.acquire()
		info = locomotion.serial_read()
		lock.release()

		if info == (-1, -1, -1): # check for invalid message
			print('ERR: Could not decode message!')

		elif info[2] == 0: # check for incorrect checksum
			print('ERR: Invalid checksum!')

		else:
			msgtype = info[0]
			msg = info[1]
			print('RECV - Type: '+ str(msgtype) + ' | Message: ' + str(msg))

if __name__ == '__main__':

	t1 = threading.Thread(target=cmd_thread)
	t2 = threading.Thread(target=read_thread)

	t1.start()
	t2.start()


