import random
import serial
import time 
import modified_protocol2 as r2p
import select
import sys
import threading, multiprocessing

sys.path.append("/home/cornellcup/c1c0-ece")
import TEST_API as sns
Lock = threading.Lock()
ser = sns.init_serial('/dev/ttyTHS1', 38400)
data = []
a = 1

def send_token(permission,addr):
    msg = r2p.encode(b"SNS", (addr).to_bytes(1,'big'),(permission.to_bytes(1,'big')))
    ser.write(msg)
    ser.flush()
    
def read_data(serialPort):
    global data
    global a
    while(1):
        send_token(1,2)
        for i in range(10):
            fio2_data = serialPort.read_until(expected = b'\xd2\xe2\xf2')
            mtype,mdata,mstatus,address = r2p.decode(fio2_data)
            data = mdata
        send_token(0,2)
        send_token(1,3)
        for i in range(10):
            fio2_data = serialPort.read_until(expected = b'\xd2\xe2\xf2')
            mtype,mdata,mstatus,address = r2p.decode(fio2_data)
            data = mdata
        send_token(0,3)

    '''
        sns.send_token(1, 200)
        for i in range(10):
            sns.decode_arrays()
            tb1 = sns.get_array("TB1")
            tb2 = sns.get_array("TB2")
            ldr = sns.get_array("LDR")
            imu = sns.get_array("IMU")
            queue.put(ldr)
        sns.send_token(0,200)
        ser.flush()
        time.sleep(.05)
        ser.reset_input_buffer()
        time.sleep(.05)
    '''
def display_data():
    global data
    global a
    time.sleep(1)
    while(True):
        print("display",data)
        time.sleep(.1)
if __name__ == "__main__":
    
    ser = serial.Serial('/dev/ttyTHS1', 38400)
    #queue = multiprocessing.Queue()	
    send_token(0,3)
    send_token(0,2)
    send_token(0,200)
    p1 = threading.Thread(target=read_data, args=(ser,))
    p2 = threading.Thread(target=display_data)
    
    p1.start()
    p2.start()
    loopcnt = 20000
    while (loopcnt > 0):
        loopcnt -= 1
        try:
            time.sleep(10)
        
        except KeyboardInterrupt:
            loopcnt=0
            
    loopcnt=0
	

