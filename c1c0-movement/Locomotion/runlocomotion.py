# new version of runlocomotion.py
# runs two threads - one which sends commands 
# to HeadLocomotion, and one which reads from
# the serial line to assure data is being received

# accepts argument for whether to run with xbox or keyboard

# there is a lock here intended for the multithreaded read/command threads,
# but the command thread currently reads itself so the lock/multiple threads
# may not actually be necessary and will probably be removed

import threading
import locomotion
import argparse

parser = argparse.ArgumentParser(description='Run locomotion w/ xbox controller or keyboard')
parser.add_argument("ctrl", help="Control C1C0 with `keyboard` or `xbox`", type=str)
args = parser.parse_args()

lock = threading.Lock()

def xboxcmd_thread():

    while True:

        lock.acquire()
        locomotion.run(1)
        lock.release()

def keycmd_thread():

    while True:

    	# catch keyboard exceptions
        try:
	        lock.acquire()
	        locomotion.key_run(1)
	        lock.release()
        except:
	    	# ensure that the motors get turned off when stopping
	        print("Exiting...")
	        lock.acquire()
	        locomotion.motor_command(0,0)
	        locomotion.head_command(0)
	        lock.release()
	        exit()


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

    t1 = threading.Thread(target=xboxcmd_thread)
    t2 = threading.Thread(target=keycmd_thread)
    t3 = threading.Thread(target=read_thread)

    if args.ctrl == 'xbox':
        t1.start()
    elif args.ctrl == 'keyboard':
        t2.start()
        while True:
            try:
                pass
            except:
                print("Exiting...")
	            lock.acquire()
	            locomotion.motor_command(0,0)
	            locomotion.head_command(0)
	            lock.release()
	            exit()
    else:
        print("runlocomotion.py: Incorrect usage - please give `xbox` or `keyboard` argument")
        exit()

    # start the read thread
    #t3.start()






