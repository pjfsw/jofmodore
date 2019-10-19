#!/usr/bin/python

import serial
import time
import fileinput

def sendCmd(cmd):
    ser.write(cmd.rstrip()+"\n")
    ser.flush()
    return ser.readline().rstrip()

port='/dev/tty.usbmodem411'
#commands=["0", "w FFF8 0123456789ABCDEF","r FFFE","w 0000 1014", "r 0001","1"]

ser = serial.Serial(port, 230400, timeout=3)
print "Sleeping for 1 second"
time.sleep(1)
print "Command sequence start"
print sendCmd("0")
for command in fileinput.input():
    print command
    print sendCmd(command)

time.sleep(1)
print sendCmd("1")
ser.close()
print "Finish"

