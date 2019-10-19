#!/usr/bin/python
import sys

if len(sys.argv) == 1:
    print "Specify input PRG file as argument"
    sys.exit(1)


filename = sys.argv[1]


fh = open(filename, 'rb')

prg = bytearray(fh.read())
address = prg[0] + prg[1] * 256
ha = "%0.4X"
hb = "%0.2X"

line = ""

for aByte in prg[2:]:
    if not line:
        line = "w " + ha % address + " "
    line = line + hb % aByte
    address = address + 1
    if len(line) >= 7 + 128:
        print line
        line = ""

if line:
    print line



#def writeBuffer:
