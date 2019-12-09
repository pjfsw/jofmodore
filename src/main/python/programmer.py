#!/usr/bin/python

import serial
import time
import fileinput

def sendCmd(cmd):
    ser.write(cmd.rstrip()+"\n")
    ser.flush()
    return ser.readline().rstrip()

def init():
    ser.write("hello\n");
    ser.flush();
    ready = False
    while not ready:
        inp = ser.readline().rstrip()
        print inp
        if inp == 'ready':
            ready = True

    return

port='/dev/tty.usbmodem411'
#commands=["0", "w FFF8 0123456789ABCDEF","r FFFE","w 0000 1014", "r 0001","1"]

ser = serial.Serial(port, 230400, timeout=10)
init()
print sendCmd("r")
for command in fileinput.input():
    print "Writing '"+command.rstrip()+"'"
    print sendCmd(command.rstrip())

time.sleep(1)
print sendCmd("b")
ser.close()
print "Finish"

