# new version of runlocomotion.py
# runs two threads - one which sends commands 
# to HeadLocomotion, and one which reads from
# the serial line to assure data is being received

# accepts argument for whether to run with xbox or keyboard

import threading
import locomotion
import argparse
import sys

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

        lock.acquire()
        locomotion.key_run(1)
        lock.release()

def read_thread():
	
    while True:

        lock.acquire()
        info = locomotion.serial_read()
        lock.release()

        if info == (-1, -1, -1): # check for invalid message
            sys.stderr.write('ERR: Could not decode message!')

        elif info[2] == 0: # check for incorrect checksum
            sys.stderr.write('ERR: Invalid checksum!')

        else:
            msgtype = info[0]
            msg = info[1]
            out = 'RECV - Type: '+ str(msgtype) + ' | Message: ' + str(msg)
            sys.stderr.write(out)

if __name__ == '__main__':

    t1 = threading.Thread(target=xboxcmd_thread)
    t2 = threading.Thread(target=keycmd_thread)
    t3 = threading.Thread(target=read_thread)

    if args.ctrl == 'xbox':
        t1.start()
    elif args.ctrl == 'keyboard':
        t2.start()
    else:
        print("runlocomotion.py: Incorrect usage - please give `xbox` or `keyboard` argument")
        exit()

    # start the read thread
    t3.start()






